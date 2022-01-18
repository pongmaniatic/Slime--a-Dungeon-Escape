#include "FGProceduralGenerator.h"

#include "FGLevelModule.h"
#include "Kismet/GameplayStatics.h"
#include "SlimePlatformer/Interface/FGItem.h"
#include "SlimePlatformer/Map Features/FGModuleDoor.h"
#include "SlimePlatformer/Map Features/FGModuleEnemy.h"
#include "SlimePlatformer/Map Features/FGModuleObstacle.h"
#include "SlimePlatformer/Map Features/FGModuleItem.h"
#include "SlimePlatformer/Systems/Followers/FG_FollowerActor.h"
#include "FGLevelModuleBackground.h"
#include "SlimePlatformer/Obstacles/FGObstacle.h"
#include "SlimePlatformer/Obstacles/FGEnemy.h"
#include "SlimePlatformer/Map Features/FGModuleWall.h"
#include "FGExitDoor.h"

DEFINE_LOG_CATEGORY(LogProceduralGeneration)

AFGProceduralGenerator::AFGProceduralGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AFGProceduralGenerator::BeginPlay()
{
	Super::BeginPlay();

	AllocateDefaultLevelModules();

	if (GenerateAtBeginPlay)
	{
		if(GenerateSequentially)
			GenerateLevelSequentially();
		else
			GenerateLevel();
	}
}

void AFGProceduralGenerator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(PlayerPosIsValid)
	{
		APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if(Pawn == nullptr)
			return;
		
		Pawn->SetActorLocation(FVector(0.f, PlayerPos.Y, PlayerPos.Z));
		PlayerPosIsValid = false;
		SetActorTickEnabled(false);
	}
}

void AFGProceduralGenerator::GenerateLevel(FVector OriginPosition)
{
	if (HasErrorsOccured())
		return;

	const float UnitStepBetweenModules = LevelDataSpecs->ModuleSize * LevelDataSpecs->TileSize;
	const int LevelExtents = (LevelDataSpecs->LevelSize - 1) / 2;
	// Level size is enforced to be uneven, extents minus center, this is because there should always be a center module

	FVector VectorOffset = FVector::UpVector;
	EDirection BaseDirection = Up;

	Levels.Add(TArray<AFGLevelModule*>());
	Levels[Levels.Num() - 1].Reserve(FMath::Square(LevelDataSpecs->LevelSize));

	int ModuleIndex = 0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = i; j <= LevelExtents; j++)
		{
			const FVector BasePosition = OriginPosition + (UnitStepBetweenModules * j) * VectorOffset;

			// Spawn modules in the middle
			SpawnRandomModule(BasePosition, ModuleIndex, {BaseDirection}, j == LevelExtents);
			ModuleIndex++;

			// Spawn modules to the left
			for (int k = 1; k <= LevelExtents; k++)
			{
				TSet<EDirection> Directions = {Left};
				
				if(j == LevelExtents)
					Directions.Add(BaseDirection);
				
				SpawnRandomModule(BasePosition + (UnitStepBetweenModules * k) * FVector::LeftVector, ModuleIndex, Directions, k == LevelExtents);
				ModuleIndex++;
			}

			// Spawn modules to the right
			for (int k = 1; k <= LevelExtents; k++)
			{
				TSet<EDirection> Directions = {Right};
				
				if(j == LevelExtents)
					Directions.Add(BaseDirection);
				
				SpawnRandomModule(BasePosition + (UnitStepBetweenModules * k) * FVector::RightVector, ModuleIndex, Directions, k == LevelExtents);
				ModuleIndex++;
			}
		}

		VectorOffset = FVector::DownVector;
		BaseDirection = Down;
	}

	for(AFGLevelModule* Module : Levels[Levels.Num() - 1])
	{
		if(Module != nullptr)
			DetermineFeatures(Module, Levels[Levels.Num() - 1].IndexOfByKey(Module));
	}

	HandleDoors();
}

void AFGProceduralGenerator::GenerateLevelSequentially(FVector OriginPosition)
{
	if (HasErrorsOccured())
		return;

	const float UnitStepBetweenModules = LevelDataSpecs->ModuleSize * LevelDataSpecs->TileSize;
	const int LevelExtents = (LevelDataSpecs->LevelSize - 1) / 2;
	// Level size is enforced to be uneven, extents minus center, this is because there should always be a center module

	FVector VectorOffset = FVector::UpVector;
	EDirection BaseDirection = Up;

	Levels.Add(TArray<AFGLevelModule*>());
	Levels[Levels.Num() - 1].Reserve(FMath::Square(LevelDataSpecs->LevelSize));

	int ModuleIndex = 0;

	FTimerManager& TimerManager = GetWorldTimerManager();

	for(int i = 0; i < FMath::Square(LevelDataSpecs->LevelSize); i++)
	{
		TimerHandles.Add(FTimerHandle());
	}

	auto FinalLambda = [this]()
	{
		for(AFGLevelModule* Module : Levels[Levels.Num() - 1])
		{
			if(Module != nullptr)
				DetermineFeatures(Module, Levels[Levels.Num() - 1].IndexOfByKey(Module));
		}

		HandleDoors();
	};

	for (int i = 0; i < 2; i++)
	{
		for (int j = i; j <= LevelExtents; j++)
		{
			const FVector BasePosition = OriginPosition + (UnitStepBetweenModules * j) * VectorOffset;

			// Spawn modules in the middle
			TimerManager.SetTimer(TimerHandles[ModuleIndex], [this, BasePosition, ModuleIndex, BaseDirection, j, LevelExtents]()
			{
				SpawnRandomModule(BasePosition, ModuleIndex, {BaseDirection}, j == LevelExtents);
			}, SequentialDelay * ModuleIndex + 0.0001f, false);
			ModuleIndex++;

			// Spawn modules to the left
			for (int k = 1; k <= LevelExtents; k++)
			{
				TSet<EDirection> Directions = {Left};
				
				if(j == LevelExtents)
					Directions.Add(BaseDirection);

				TimerManager.SetTimer(TimerHandles[ModuleIndex], [this, BasePosition, ModuleIndex, Directions, UnitStepBetweenModules, k, LevelExtents]()
				{
					SpawnRandomModule(BasePosition + (UnitStepBetweenModules * k) * FVector::LeftVector, ModuleIndex, Directions, k == LevelExtents);
				}, SequentialDelay * ModuleIndex, false);
				
				ModuleIndex++;
			}

			// Spawn modules to the right
			for (int k = 1; k <= LevelExtents; k++)
			{
				TSet<EDirection> Directions = {Right};
				
				if(j == LevelExtents)
					Directions.Add(BaseDirection);

				TimerManager.SetTimer(TimerHandles[ModuleIndex], [this, BasePosition, ModuleIndex, Directions, UnitStepBetweenModules, k, LevelExtents, FinalLambda]()
				{
					SpawnRandomModule(BasePosition + (UnitStepBetweenModules * k) * FVector::RightVector, ModuleIndex, Directions, k == LevelExtents);
					if(ModuleIndex == FMath::Square(LevelDataSpecs->LevelSize) - 1)
					{
						FinalLambda();
					}
				}, SequentialDelay * ModuleIndex, false);
				
				ModuleIndex++;
			}
		}

		VectorOffset = FVector::DownVector;
		BaseDirection = Down;
	}
}

void AFGProceduralGenerator::HandleDoors()
{
	if(Doors.Num() < 2)
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("There are less than 2 doors in the entire level so an entrance/exit cannot be chosen"));
		return;
	}

	const int LevelSize = LevelDataSpecs->LevelSize;
	const int LevelExtents = (LevelSize - 1) / 2;
	
	TArray<int> Keys;
	Doors.GetKeys(Keys);
	int CurrentDoorKey;

	const int TopIndex = LevelSize * LevelExtents + LevelExtents;
	const int BottomIndex = LevelSize * LevelExtents * 2 + LevelExtents;
	
	// Determine spawn location (prefer top left or top right corner)
	UFGModuleDoor* SpawnDoor = ChooseRandomDoorInCorner(StartFromTop ? TopIndex : BottomIndex, Keys, CurrentDoorKey);

	PlayerPos = SpawnDoor->GetComponentLocation();
	PlayerPosIsValid = true;
	SetActorTickEnabled(true);
	UE_LOG(LogProceduralGeneration, Log, TEXT("Player spawn location, x: %f, y: %f, z: %f"), PlayerPos.X, PlayerPos.Y, PlayerPos.Z);
	
	Keys.Remove(CurrentDoorKey);

	// Determine exit location (prefer bottom left or bottom right corner)
	UFGModuleDoor* ExitDoor = ChooseRandomDoorInCorner(StartFromTop ? BottomIndex : TopIndex, Keys, CurrentDoorKey);
	AActor* Door = GetWorld()->SpawnActor(LevelDataSpecs->ExitDoorBlueprint);
	if(Door == nullptr)
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("Door couldn't spawn, this is really cursed"));
		return;
	}
	const FVector Pos = ExitDoor->GetComponentLocation();
	Door->SetActorLocation(FVector(0.f, Pos.Y, Pos.Z));
}

UFGModuleDoor* AFGProceduralGenerator::ChooseRandomDoorInCorner(int LeftCornerIndex, const TArray<int>& Keys, int& DoorKey)
{
	const int LevelSize = LevelDataSpecs->LevelSize;
	const int LevelExtents = (LevelSize - 1) / 2;
	
	DoorKey = -1;
	DoorKey = LeftCornerIndex;
	if(!Doors.Contains(DoorKey))
	{
		DoorKey += LevelExtents;
		if(!Doors.Contains(DoorKey))
		{
			int RandomKeyIndex = FMath::RandRange(0, Keys.Num() - 1);

			DoorKey = Keys[RandomKeyIndex];
		}
	}
	else if(Doors.Contains(DoorKey + LevelExtents) && FMath::RandBool())
		DoorKey += LevelExtents;

	TArray<UFGModuleDoor*> SpawnDoors = Doors[DoorKey];
	return SpawnDoors[FMath::RandRange(0, SpawnDoors.Num() - 1)];
}

void AFGProceduralGenerator::DetermineFeatures(AFGLevelModule* Module, int ModuleIndex)
{
	TArray<UFGModuleFeature*> Features;
	Module->GetComponents<UFGModuleFeature>(Features);

	for(UFGModuleFeature* Feature : Features)
	{
		UFGModuleItem* Item = Cast<UFGModuleItem>(Feature);
		if(Item != nullptr)
		{
			DetermineItem(Module, Item);
			continue;
		}

		UFGModuleObstacle* Obstacle = Cast<UFGModuleObstacle>(Feature);
		if(Obstacle != nullptr)
		{
			SpawnRandomBasedOnWeights<FObstacleWeights, AActor>(this, LevelDataSpecs->ObstacleWeights, Obstacle->GetComponentLocation());
			continue;
		}

		UFGModuleEnemy* Enemy = Cast<UFGModuleEnemy>(Feature);
		if(Enemy != nullptr)
		{
			SpawnRandomBasedOnWeights<FEnemyWeights, AActor>(this, LevelDataSpecs->EnemyWeights, Enemy->GetComponentLocation());
			continue;
		}

		UFGModuleDoor* Door = Cast<UFGModuleDoor>(Feature);
		if(Door != nullptr)
		{
			if(!Doors.Contains(ModuleIndex))
			{
				Doors.Add(ModuleIndex, {Door});
			}
			else
			{
				Doors.Find(ModuleIndex)->Add(Door);
			}
			continue;
		}
	}
}

void AFGProceduralGenerator::DetermineItem(AFGLevelModule* Module, UFGModuleItem* Item)
{
	const float CombinedPercentage = Item->PercentageOfBeginHealth + Item->PercentageOfBeingACoin;
	if(CombinedPercentage >= 1.f)
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("Can't determine item since percentage of being health + percentage of being a coin is equal to or exceeds 1/100%"))
		return;
	}

	const float PercentageOfBeingARegularItem = 1.f - CombinedPercentage;

	const float Random = FMath::FRand();

	AActor* Actor = nullptr;
	if(Random < PercentageOfBeingARegularItem)
	{
		// Regular item
		SpawnRandomBasedOnWeights<FItemWeights, AActor>(this, LevelDataSpecs->ItemWeights, Item->GetComponentLocation());
	}
	else if(Random < PercentageOfBeingARegularItem + Item->PercentageOfBeginHealth)
	{
		// Health
		Actor = GetWorld()->SpawnActor(*LevelDataSpecs->HealthBlueprint);
	}
	else
	{
		// Coin
		Actor = GetWorld()->SpawnActor(*LevelDataSpecs->MoneyBlueprint);
	}
	
	if(Actor != nullptr)
	{
		const FVector Pos = Item->GetComponentLocation();
		Actor->SetActorLocation(FVector(0.f, Pos.Y, Pos.Z));
	}
}

template <class T, class ReturnType>
ReturnType* AFGProceduralGenerator::SpawnRandomBasedOnWeights(UObject* WorldContextObject, TArray<T>& WeightStruct, FVector Position)
{
	UClass* Class = GetRandomClassBasedOnWeights<T>(WorldContextObject, WeightStruct);
	AActor* Actor = WorldContextObject->GetWorld()->SpawnActor(Class);
	if(Actor == nullptr)
	{
		UE_LOG(LogProceduralGeneration, Warning, TEXT("This should never happen, yet it did"));
		return nullptr;
	}
	Actor->SetActorLocation(FVector(0.f, Position.Y, Position.Z));
	return Cast<ReturnType>(Actor);
}

template <class T>
UClass* AFGProceduralGenerator::GetRandomClassBasedOnWeights(UObject* WorldContextObject, TArray<T>& WeightStruct)
{
	TArray<T> Weights = WeightStruct;

	float TotalWeight = 0.f; // Divide weights by total amount of all weights to get percentage
	for(T Weight : Weights)
	{
		TotalWeight += Weight.Weight;
	}

	const float Random = FMath::FRand();
	float PreviousPercentage = 0.f;

	for(T Weight : Weights)
	{
		PreviousPercentage += Weight.Weight / TotalWeight;

		if(Random < PreviousPercentage)
		{
			if(Weight.Class == nullptr)
			{
				UE_LOG(LogProceduralGeneration, Warning, TEXT("This should never happen, yet it did"));
				return nullptr;
			}
			return Weight.Class;
		}
	}
	return nullptr;
}

void AFGProceduralGenerator::DeleteLevel(int Index)
{
	if (Index < 0 || Index >= Levels.Num())
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("Couldn't delete level at index %i"), Index);
		return;
	}

	for (int i = 0; i < Levels[Index].Num(); i++)
	{
		if (Levels[Index][i] != nullptr)
			Levels[Index][i]->Destroy();
	}
	Levels.RemoveAt(Index);
}

AFGItem* AFGProceduralGenerator::SpawnRandomItem(UObject* WorldContextObject, TArray<FItemWeights> WeightStruct, FVector Position)
{
	return SpawnRandomBasedOnWeights<FItemWeights, AFGItem>(WorldContextObject, WeightStruct, Position);
}

UClass* AFGProceduralGenerator::GetRandomItemClass(UObject* WorldContextObject, TArray<FItemWeights> WeightStruct)
{
	return GetRandomClassBasedOnWeights<FItemWeights>(WorldContextObject, WeightStruct);
}

bool AFGProceduralGenerator::HasErrorsOccured()
{
	if (LevelModules.Num() < 1)
	{
		UE_LOG(LogProceduralGeneration, Error,
		       TEXT("Couldn't generate level, there are no modules assigned in the array"));
		return true;
	}
	if (LevelDataSpecs == nullptr)
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("Couldn't generate level, there is no level data specs assigned"));
		return true;
	}
	if (LevelDataSpecs->LevelSize < 1)
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("Couldn't generate level, level size is below one"));
		return true;
	}
	if (LevelDataSpecs->LevelSize % 2 == 0)
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("Couldn't generate level, level size is even, has to be uneven"));
		return true;
	}
	if (LevelDataSpecs->TileSize < 1)
	{
		UE_LOG(LogProceduralGeneration, Error, TEXT("Couldn't generate level, tile size is below one"));
		return true;
	}
	return false;
}

void AFGProceduralGenerator::AllocateDefaultLevelModules()
{
	DefaultLevelModules.Empty();
	DefaultLevelModules.Reserve(LevelModules.Num());

	for (int i = 0; i < LevelModules.Num(); i++)
	{
		DefaultLevelModules.Add(Cast<AFGLevelModule>(LevelModules[i].GetDefaultObject()));
	}
}

void AFGProceduralGenerator::SpawnRandomModule(FVector Position, int ModuleIndex, TSet<EDirection> Directions, bool IsOnEdge)
{
	TArray<TSubclassOf<AFGLevelModule>> RelevantLevelModules;
	TArray<FNeighborInfo> NeighborInfo = GetRelevantLevelModules(ModuleIndex, RelevantLevelModules);

	const int NumberOfModules = RelevantLevelModules.Num();

	if (NumberOfModules == 0)
	{
		UE_LOG(LogProceduralGeneration, Warning, TEXT("Couldn't generate module at index %i, no valid modules found"),
		       ModuleIndex);
		Levels[Levels.Num() - 1].Add(nullptr);

		for(FNeighborInfo Info : NeighborInfo)
		{
			// Spawn corridor block on neigbors so that it doesn't lead to nowhere
			SpawnCorridorBlock(Info.Module, Reverse(Info.ModuleDirection));
		}
		
		return;
	}

	const int SelectedModule = FMath::RandRange(0, NumberOfModules - 1);
	AFGLevelModule* Module = Cast<AFGLevelModule>(GetWorld()->SpawnActor(RelevantLevelModules[SelectedModule], &Position));

	if(LevelModuleBackgrounds.Num() > 0)
	{
		const int SelectedBackground = FMath::RandRange(0, LevelModuleBackgrounds.Num() - 1);
		const FVector BackgroundPos = Position + FVector::ForwardVector * BackgroundTileOffset * LevelDataSpecs->TileSize;
		GetWorld()->SpawnActor(LevelModuleBackgrounds[SelectedBackground], &BackgroundPos);
	}

	for(FNeighborInfo Info : NeighborInfo)
	{
		if(Info.Module == nullptr)
		{
			SpawnCorridorBlock(Module, Info.ModuleDirection);
		}
	}
	
	Levels[Levels.Num() - 1].Add(Module);

	if(IsOnEdge)
	{
		for(EDirection Dir : Directions)
		{
			SpawnCorridorBlock(Module, Dir);
		}
	}
}

void AFGProceduralGenerator::SpawnCorridorBlock(AFGLevelModule* Module, EDirection Direction)
{
	if(Module == nullptr)
		return;

	const int Location = Module->DoorLocations[Direction];
	const int ModuleSize = LevelDataSpecs->ModuleSize;

	if(Location < 0)
		return;

	const float MidOffset = ModuleSize * 0.5f - 0.5;
	const float Offset = Location - MidOffset;

	FVector Position = FVector::ZeroVector;

	switch(Direction)
	{
	case None:
		break;
	case Up:
		Position += Offset * FVector::RightVector + MidOffset * FVector::UpVector;
		break;
	case Down:
		Position += Offset * FVector::RightVector + MidOffset * FVector::DownVector;
		break;
	case Left:
		Position += Offset * FVector::DownVector + MidOffset * FVector::LeftVector;
		break;
	case Right:
		Position += Offset * FVector::DownVector + MidOffset * FVector::RightVector;
		break;
	default: ;
	}
	
	for(int i = 0; i < LevelDataSpecs->CorridorWidth; i++)
	{
		UE_LOG(LogProceduralGeneration, Log, TEXT("Tried to add module wall to block corridor in module index: %i with relative coordinates x: %f, y: %f, z: %f"), Levels[Levels.Num() - 1].IndexOfByKey(Module), Position.X, Position.Y, Position.Z);
		Module->AddComponentByClass(*LevelDataSpecs->ModuleWall, false, FTransform(Position * LevelDataSpecs->TileSize), false);

		if(Direction == Right || Direction == Left)
			Position += FVector::DownVector;
		else
			Position += FVector::RightVector;
	}
}

TArray<FNeighborInfo> AFGProceduralGenerator::GetRelevantLevelModules(int ModuleIndex, TArray<TSubclassOf<AFGLevelModule>>& RelevantLevelModules)
{
	RelevantLevelModules = LevelModules;
	TArray<AFGLevelModule*> CurrentDefaultModules = DefaultLevelModules;

	if (ModuleIndex == 0)
		return TArray<FNeighborInfo>();

	TArray<FNeighborInfo> NeighborInfo;
	GetNeighborInfo(ModuleIndex, NeighborInfo);

	UE_LOG(LogProceduralGeneration, Log, TEXT("Generating module with index %i"), ModuleIndex);

	for (FNeighborInfo Info : NeighborInfo)
	{
		if (Info.Module == nullptr)
			continue;

		LogNeighborInfo(Info);

		for (int i = 0; i < RelevantLevelModules.Num();)
		{
			if (Info.Module->DoorLocations[Reverse(Info.ModuleDirection)] != CurrentDefaultModules[i]->DoorLocations[
				Info.ModuleDirection])
			{
				RelevantLevelModules.RemoveAt(i);
				CurrentDefaultModules.RemoveAt(i);
				continue;
			}
			i++;
		}
	}
	return NeighborInfo;
}

void AFGProceduralGenerator::LogNeighborInfo(const FNeighborInfo& Info)
{
	FString TempLogString;
	switch (Info.ModuleDirection)
	{
	case Up: TempLogString = TEXT("Up");
		break;
	case Down: TempLogString = TEXT("Down");
		break;
	case Left: TempLogString = TEXT("Left");
		break;
	case Right: TempLogString = TEXT("Right");
		break;
	default: TempLogString = TEXT("None");
	}
	UE_LOG(LogProceduralGeneration, Log, TEXT("Neighbor: %i, Direction: %s"),
		Levels[Levels.Num() - 1].IndexOfByKey(Info.Module), *TempLogString);
}

void AFGProceduralGenerator::GetNeighborInfo(int ModuleIndex, TArray<FNeighborInfo>& NeighborInfo)
{
	const int LevelSize = LevelDataSpecs->LevelSize;
	const int LevelExtents = (LevelSize - 1) / 2;

	// Middle column
	if (ModuleIndex % LevelSize == 0)
	{
		if (ModuleIndex <= LevelSize * LevelExtents)
		{
			// Middle column top section
			NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - LevelSize], Down));
		}
		else
		{
			// Middle column bottom section
			if (ModuleIndex == LevelSize * (LevelExtents + 1))
			{
				// If first in middle column bottom section, index 0 will be right above it
				NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][0], Up));
			}
			else
			{
				NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - LevelSize], Up));
			}
		}
	}
	else
	{
		if (ModuleIndex % LevelSize <= LevelExtents) // Left section
		{
			if (ModuleIndex <= LevelSize * LevelExtents + LevelExtents)
			{
				// Top left section
				NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - 1], Right));

				if (ModuleIndex > LevelExtents) // If not Left middle row
				{
					NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - LevelSize], Down));
				}
			}
			else
			{
				// Bottom left section
				NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - 1], Right));

				if (ModuleIndex <= LevelSize * (LevelExtents + 1) + LevelExtents)
				{
					// If first row in bottom left section
					NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex % LevelSize], Up));
				}
				else
				{
					NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - LevelSize], Up));
				}
			}
		}
		else // Right section
		{
			if (ModuleIndex % LevelSize == LevelExtents + 1) // If first right column
			{
				NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - LevelExtents - 1], Left));
			}
			else
			{
				NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - 1], Left));
			}

			if (ModuleIndex <= LevelSize * LevelExtents + LevelExtents * 2)
			{
				// Top right section + right middle row
				if (ModuleIndex > LevelExtents * 2) // If not middle row
				{
					NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - LevelSize], Down));
				}
			}
			else
			{
				// Bottom right section
				if (ModuleIndex <= LevelSize * (LevelExtents + 1) + LevelExtents * 2)
					// If first row in bottom right section
				{
					NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex % LevelSize], Up));
				}
				else
				{
					NeighborInfo.Add(FNeighborInfo(Levels[Levels.Num() - 1][ModuleIndex - LevelSize], Up));
				}
			}
		}
	}
}

EDirection AFGProceduralGenerator::Reverse(EDirection Direction)
{
	switch (Direction)
	{
	case Up: return Down;
	case Down: return Up;
	case Left: return Right;
	case Right: return Left;
	default: return None;
	}
}

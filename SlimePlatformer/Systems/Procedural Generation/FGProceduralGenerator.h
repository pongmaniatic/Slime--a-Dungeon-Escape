#pragma once
#include "GameFramework/Actor.h"
#include "FGLevelDataSpecs.h"
#include "FGProceduralGenerator.generated.h"

class AFGItem;
class AFGLevelModuleBackground;
class UFGModuleDoor;
class UFGModuleFeature;
class UFGModuleItem;
class UFGModuleObstacle;
class UFGLevelDataSpecs;
class AFGLevelModule;

DECLARE_LOG_CATEGORY_EXTERN(LogProceduralGeneration, Log, All);

enum EDirection
{
	None = -1,
	Up,
	Down,
	Left,
	Right
};

struct FNeighborInfo
{
	FNeighborInfo(AFGLevelModule* Module, EDirection ModuleDirection)
	{
		this->Module = Module;
		this->ModuleDirection = ModuleDirection;
	}
	
	AFGLevelModule* Module;
	EDirection ModuleDirection;
};

UCLASS()
class AFGProceduralGenerator : public AActor
{
	GENERATED_BODY()

public:
	AFGProceduralGenerator();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	bool GenerateAtBeginPlay = true;

	/** If this and generate at begin play is true, the level will be generated per module with a specified delay between */
	UPROPERTY(EditAnywhere)
	bool GenerateSequentially = false;

	UPROPERTY(EditAnywhere)
	float SequentialDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AFGLevelModule>> LevelModules;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AFGLevelModuleBackground>> LevelModuleBackgrounds;

	/** The locational offset in tiles in the x direction for the background compared to the level module */
	UPROPERTY(EditAnywhere)
	float BackgroundTileOffset = 1;

	UPROPERTY(EditAnywhere)
	UFGLevelDataSpecs* LevelDataSpecs;

	/** Generates the level with the specified location as an origin with the modules defined in level modules and data from level data specs */
	UFUNCTION(BlueprintCallable)
	void GenerateLevel(FVector OriginPosition = FVector::ZeroVector);

	/** Not practical to use in the normal game, only for debug purposes */
	UFUNCTION(BlueprintCallable)
	void GenerateLevelSequentially(FVector OriginPosition = FVector::ZeroVector);

	/** Deletes a level at the specified index (every call to generate level will add a level to an array)*/
	UFUNCTION(BlueprintCallable)
	void DeleteLevel(int Index);

	/** Start from top and exit at bottom, or start from bottom and exit at top */
	UPROPERTY(EditAnywhere)
	bool StartFromTop = false;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static AFGItem* SpawnRandomItem(UObject* WorldContextObject, TArray<FItemWeights> WeightStruct, FVector Position);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UClass* GetRandomItemClass(UObject* WorldContextObject, TArray<FItemWeights> WeightStruct);

private:
	TArray<TArray<AFGLevelModule*>> Levels;

	UPROPERTY()
	TArray<AFGLevelModule*> DefaultLevelModules;

	TArray<FTimerHandle> TimerHandles;
	
	bool HasErrorsOccured();
	void SpawnRandomModule(FVector Position, int ModuleIndex, TSet<EDirection> Directions, bool IsOnEdge = false);
	void AllocateDefaultLevelModules();
	TArray<FNeighborInfo> GetRelevantLevelModules(int ModuleIndex, TArray<TSubclassOf<AFGLevelModule>>& RelevantLevelModules);
	void DetermineFeatures(AFGLevelModule* Module, int ModuleIndex);
	void DetermineItem(AFGLevelModule* Module, UFGModuleItem* Item);
	void LogNeighborInfo(const FNeighborInfo& Info);
	void GetNeighborInfo(int ModuleIndex, TArray<FNeighborInfo>& NeighborInfo);
	void SpawnCorridorBlock(AFGLevelModule* Module, EDirection Direction);
	void HandleDoors();
	UFGModuleDoor* ChooseRandomDoorInCorner(int LeftCornerIndex, const TArray<int>& Keys, int& DoorKey);
	static EDirection Reverse(EDirection Direction);

	template<class T, class ReturnType>
	static ReturnType* SpawnRandomBasedOnWeights(UObject* WorldContextObject, TArray<T>& WeightStruct, FVector Position);

	template<class T>
	static UClass* GetRandomClassBasedOnWeights(UObject* WorldContextObject, TArray<T>& WeightStruct);

	TMap<int, TArray<UFGModuleDoor*>> Doors;

	FVector PlayerPos;
	bool PlayerPosIsValid = false;
};

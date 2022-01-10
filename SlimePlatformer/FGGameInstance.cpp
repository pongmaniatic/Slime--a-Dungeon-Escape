#include "FGGameInstance.h"
#include "FGSlimeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Systems/Followers/FG_FollowerSystem.h"

UFGGameInstance* UFGGameInstance::CachedInstance = nullptr;

UFGGameInstance::UFGGameInstance()
{
	
}

void UFGGameInstance::OnStart()
{
	Super::OnStart();

	FWorldDelegates::OnWorldBeginTearDown.AddStatic(OnBeginTearingDown);
	FWorldDelegates::OnWorldTickStart.AddStatic(OnWorldTickStart);
	SetMusicVolume(MusicVolume);
	SetSFXVolume(SfxVolume);
	InvalidateData();
}

void UFGGameInstance::SetMusicVolume(float Volume)
{
	MusicVolume = Volume;
	MusicClass->Properties.Volume = Volume;
}

void UFGGameInstance::SetSFXVolume(float Volume)
{
	SfxVolume = Volume;
	SFXClass->Properties.Volume = Volume;
}

void UFGGameInstance::ClearPlayerStats(UObject* WorldContextObject)
{
	UFGGameInstance* Instance = GetGameInstance(WorldContextObject);
	Instance->ShouldClearPlayerStats = true;
	Instance->InvalidateData();
}

UFGGameInstance* UFGGameInstance::GetGameInstance(UObject* WorldContextObject)
{
	if(!CachedInstance)
		CachedInstance = Cast<UFGGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	return CachedInstance;
}

void UFGGameInstance::OnBeginTearingDown(UWorld* World)
{
	UFGGameInstance* Instance = GetGameInstance(World);
	
	if(Instance->ShouldClearPlayerStats)
	{
		Instance->ShouldClearPlayerStats = false;
		return;
	}

	if(World->GetFName() == Instance->PlayerData.OldLevelName)
		return;
	
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if(Pawn == nullptr)
		return;
		
	const AFGSlimeCharacter* OldCharacter = Cast<AFGSlimeCharacter>(Pawn);
	if(OldCharacter == nullptr)
		return;
		
	UFG_FollowerSystem* FollowerSystem = OldCharacter->FindComponentByClass<UFG_FollowerSystem>();
	Instance->PlayerData.Coins = FollowerSystem->coinsNumber;
	Instance->PlayerData.Lives = FollowerSystem->lifesNumber;

	for(AActor* Actor : OldCharacter->currentUpgrades)
	{
		if(Actor == nullptr)
			continue;
		
		Instance->PlayerData.UpgradeClasses.Add(Actor->GetClass());
	}
	
	if(OldCharacter->heldItem != nullptr)
		Instance->PlayerData.ItemClass = OldCharacter->heldItem->GetClass();
	
	Instance->PlayerData.OldLevelName = World->GetFName();
	Instance->PlayerData.DataIsValid = true;
}

void UFGGameInstance::OnWorldTickStart(UWorld* World, ELevelTick LevelTick, float DeltaSeconds)
{
	UFGGameInstance* Instance = GetGameInstance(World);
	
	if(!Instance->PlayerData.DataIsValid)
		return;
	
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if(Pawn == nullptr)
		return;
		
	AFGSlimeCharacter* NewCharacter = Cast<AFGSlimeCharacter>(Pawn);
	if(NewCharacter == nullptr)
		return;
	
	UFG_FollowerSystem* FollowerSystem = NewCharacter->FindComponentByClass<UFG_FollowerSystem>();
	FollowerSystem->toRememberCoinsNumber = Instance->PlayerData.Coins;
	FollowerSystem->toRememberlifesNumber = Instance->PlayerData.Lives;
	FollowerSystem->ReCreateFollowers();

	if(Instance->PlayerData.ItemClass != nullptr)
		NewCharacter->toReCreateItem = Instance->PlayerData.ItemClass;

	if(Instance->PlayerData.UpgradeClasses.Num() > 0)
	{
		for(int i = 0; i < Instance->PlayerData.UpgradeClasses.Num(); i++)
		{
			UClass* Class = Instance->PlayerData.UpgradeClasses[i];
			
			if(Class == nullptr)
				continue;
			
			NewCharacter->toReCreateUpgrades.Add(Class);
		}
		NewCharacter->ReCreateUpgrades();
	}
	
	if(NewCharacter->toReCreateItem != nullptr)
		NewCharacter->ReCreateItem();
	
	Instance->InvalidateData();
}

void UFGGameInstance::InvalidateData()
{
	PlayerData.UpgradeClasses.Empty();
	PlayerData.DataIsValid = false;
	PlayerData.OldLevelName = TEXT("");
	PlayerData.Coins = 0;
	PlayerData.Lives = 0;
	PlayerData.ItemClass = nullptr;
}

 FString UFGGameInstance::floatToClock(int numberOfSeconds)
{

	int minutes = 0;
	int seconds = 0;
	while (numberOfSeconds >= 60)
	{
		numberOfSeconds -= 60;
		minutes += 1;
	}
	seconds = numberOfSeconds;

	FString minutesString;
	FString secondsString;
	if (minutes < 10) 
	{
		 minutesString = "0" + FString::FromInt(minutes);
	}
	else
	{
		 minutesString = FString::FromInt(minutes);
	}
	
	if (seconds < 10)
	{
		 secondsString = "0" + FString::FromInt(seconds);
	}
	else
	{
		 secondsString = FString::FromInt(seconds);
	}


	FString timeString = minutesString + ":" + secondsString;

	return timeString;

}

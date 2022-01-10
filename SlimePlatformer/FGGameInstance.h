#pragma once
#include "Engine/GameInstance.h"
#include "FGGameInstance.generated.h"

class AFGItem;

USTRUCT()
struct FPlayerData
{
	GENERATED_BODY()
	
	int Lives = 0;
	int Coins = 0;

	UPROPERTY(Transient)
	UClass* ItemClass;

	UPROPERTY(Transient)
	TArray<UClass*> UpgradeClasses;
	
	FName OldLevelName;
	bool DataIsValid = false;
};

USTRUCT(BlueprintType)
struct FPlayerScore
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int lv1Time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int lv2Time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int lv3Time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int GameTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int mostCoins;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int currentCoins;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool notHurtOnce;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool hurt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool coinGrabbed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool notGreedyOnce;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool tripleJumpDiscovered;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool doubleSwordDiscovered;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool swordProjectileDiscovered;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool doubleProjectileDiscovered;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool dashSwordDiscovered;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool doubleDashDiscovered;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool swordJumpDiscovered;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool doubleJumpAndDashDiscovered;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool shootSwordDiscovered;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool dashProjectileDiscovered;
};

UCLASS()
class UFGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFGGameInstance();
	virtual void OnStart() override;

	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float Volume);

	UPROPERTY(EditAnywhere)
	USoundClass* SFXClass;

	UPROPERTY(EditAnywhere)
	USoundClass* MusicClass;

	UPROPERTY(BlueprintReadOnly)
	float MusicVolume = 0.5f;

	UPROPERTY(BlueprintReadOnly)
	float SfxVolume = 0.5f;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ClearPlayerStats(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	FString floatToClock(int numberOfSeconds);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPlayerScore PlayerScore;

private:
	UPROPERTY(Transient)
	FPlayerData PlayerData;

	static void OnBeginTearingDown(UWorld* World);
	static void OnWorldTickStart(UWorld* World, ELevelTick LevelTick, float DeltaSeconds);
	static UFGGameInstance* GetGameInstance(UObject* WorldContextObject);

	static UFGGameInstance* CachedInstance;

	bool ShouldClearPlayerStats;
	void InvalidateData();
};

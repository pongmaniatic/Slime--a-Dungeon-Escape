// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Vector.h"
#include "FG_FollowerActor.h"
#include "FG_FollowerSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLIMEPLATFORMER_API UFG_FollowerSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFG_FollowerSystem();

	void UpdatePosArray();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

	UFUNCTION(BlueprintCallable)
	void UpdateCoin(int aumountOfCoin);

	UFUNCTION(BlueprintCallable)
	void UpdateLife(int aumountOfLife);

	UFUNCTION(BlueprintCallable)
	void ReCreateFollowers();

	UFUNCTION(BlueprintCallable)
	void Spawn(TSubclassOf<class AFG_FollowerActor> toSpawn);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> playerPosPath;
	
	UPROPERTY(VisibleAnywhere)
	APawn* player;

	UPROPERTY(VisibleAnywhere)
	FVector startingVector;

	UPROPERTY(VisibleAnywhere)
	bool TrackingPos = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = tracking)
	int lengthOfPosTracking = 300;

	UPROPERTY(BlueprintReadWrite)
	int coinsNumber = 0;

	UPROPERTY(BlueprintReadWrite)
	int lifesNumber = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int toRememberCoinsNumber = 0;

	UPROPERTY(BlueprintReadWrite)
	int toRememberlifesNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int coinMax = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int lifeMax = 3;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFG_FollowerActor> coinFollower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AFG_FollowerActor> lifeFollower;
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../FGSlimeCharacter.h"
#include "FG_FollowerActor.generated.h"

UCLASS()
class SLIMEPLATFORMER_API AFG_FollowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFG_FollowerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintImplementableEvent)
		void ReSetFollower();

	UPROPERTY(BlueprintReadWrite, Category = Upgrades)
	int price = 0;
	
	UPROPERTY(BlueprintReadWrite, Category = Upgrades)
	bool bought = false;

	UPROPERTY(BlueprintReadWrite, Category = Upgrades)
	int position;

	UPROPERTY(BlueprintReadWrite, Category = Upgrades)
	AFGSlimeCharacter* slimeCharacter;


};

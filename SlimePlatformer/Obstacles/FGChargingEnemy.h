// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGEnemy.h"
#include "FGChargingEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SLIMEPLATFORMER_API AFGChargingEnemy : public AFGEnemy
{
	GENERATED_BODY()
	
public:
	AFGChargingEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> TurnTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LineTraceLength = 50.f;

private:
	void LineTraceCheck();
};

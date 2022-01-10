// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../Interface/FGItem.h"
#include "FG_BaseRecepies.generated.h"

/**
 * 
 */
UCLASS()
class SLIMEPLATFORMER_API UFG_BaseRecepies : public UDataAsset
{
	GENERATED_BODY()
	
public:


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString itemAName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString itemBName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AFGItem> itemC;


};

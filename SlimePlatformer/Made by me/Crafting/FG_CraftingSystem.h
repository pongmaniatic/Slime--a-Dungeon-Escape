// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FG_BaseRecepies.h"
#include "../Followers/FG_FollowerActor.h"
#include "../../Interface/FGItem.h"
#include "FG_CraftingSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLIMEPLATFORMER_API UFG_CraftingSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_CraftingSystem();






protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	bool Craft();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Crafting)
		TArray<UFG_BaseRecepies*> AllRecepies;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Crafting)
		AFGItem* currentlyHeldItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Crafting)
		AFGItem* newHeldItem;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Crafting)
		AFGItem* ItemToCraft;
		
};

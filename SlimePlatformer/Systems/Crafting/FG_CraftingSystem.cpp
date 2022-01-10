// Fill out your copyright notice in the Description page of Project Settings.


#include "FG_CraftingSystem.h"

// Sets default values for this component's properties
UFG_CraftingSystem::UFG_CraftingSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFG_CraftingSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFG_CraftingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool UFG_CraftingSystem::Craft()
{
	if (currentlyHeldItem && newHeldItem)
	{
		for (size_t i = 0; i < AllRecepies.Num(); i++)
		{
			if(currentlyHeldItem->itemName == AllRecepies[i]->itemAName && newHeldItem->itemName == AllRecepies[i]->itemBName ||
				newHeldItem->itemName == AllRecepies[i]->itemAName && currentlyHeldItem->itemName == AllRecepies[i]->itemBName)
			{
				UWorld* world = GetWorld();
				if (world)
				{
					FActorSpawnParameters spawnParams;
					spawnParams.Owner = GetOwner();
					FRotator rotator;
					FVector spawnLocation = GetOwner()->GetActorLocation();
					AFGItem* craftedItem = world->SpawnActor<AFGItem>(AllRecepies[i]->itemC, spawnLocation, rotator, spawnParams);
					ItemToCraft = craftedItem;
					return true;
				}
			}
		}
	}
	return false;
}


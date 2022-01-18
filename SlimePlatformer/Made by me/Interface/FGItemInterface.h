// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FGItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFGItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLIMEPLATFORMER_API IFGItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void JumpAbility();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void ActiveAbility();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void DiscardAbility();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void PickUp(AActor* caller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void Drop(AActor* caller);

};

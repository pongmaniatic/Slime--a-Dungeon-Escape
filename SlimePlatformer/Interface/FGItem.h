// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "FGItem.generated.h"

UCLASS()
class SLIMEPLATFORMER_API AFGItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFGItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void ReSetItem();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* BaseCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
		FString itemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
		bool currentlyBeingHeld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = player)
		AActor*  slimePlayer; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = player)
		FString  text; 
};

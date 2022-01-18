// Fill out your copyright notice in the Description page of Project Settings.


#include "FG_FollowerActor.h"

// Sets default values
AFG_FollowerActor::AFG_FollowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFG_FollowerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFG_FollowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


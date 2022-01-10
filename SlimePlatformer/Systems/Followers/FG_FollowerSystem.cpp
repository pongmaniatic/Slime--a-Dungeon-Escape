
#include "FG_FollowerSystem.h"
#include "Math/Vector.h"
#include "../../FGSlimeCharacter.h"

// Sets default values for this component's properties
UFG_FollowerSystem::UFG_FollowerSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFG_FollowerSystem::BeginPlay()
{
	Super::BeginPlay();

	startingVector = Cast<AFGSlimeCharacter>(GetOwner())->GetActorLocation();
	playerPosPath.Init(startingVector, lengthOfPosTracking);

}

void UFG_FollowerSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdatePosArray();

}

void UFG_FollowerSystem::UpdatePosArray()
{
	playerPosPath[lengthOfPosTracking - 1] = Cast<AFGSlimeCharacter>(GetOwner())->GetActorLocation();
	for (int i = 0; i < lengthOfPosTracking-1; i++){if (i < lengthOfPosTracking - 1){playerPosPath[i] = playerPosPath[i + 1];}}
	FVector VectorExample = playerPosPath[0];
}

void UFG_FollowerSystem::UpdateCoin(int aumountOfCoin)
{
	coinsNumber += aumountOfCoin;
}

void UFG_FollowerSystem::UpdateLife(int aumountOfLife)
{
	if (aumountOfLife < 0) 
	{
		if (!Cast<AFGSlimeCharacter>(GetOwner())->invulnerable)
		{
			lifesNumber += aumountOfLife;
		}
	}
	else 
	{
		lifesNumber += aumountOfLife;
	}

}

void UFG_FollowerSystem::ReCreateFollowers()
{
	coinsNumber = 0;
	lifesNumber = 0;
	for (size_t i = 0; i < toRememberCoinsNumber; i++)
	{
		
		if (coinFollower) {  Spawn(coinFollower); }
	}

	for (size_t i = 0; i < toRememberlifesNumber; i++)
	{
		if (lifeFollower) {  Spawn(lifeFollower); }
	}

}

void UFG_FollowerSystem::Spawn(TSubclassOf<class AFG_FollowerActor> toSpawn)
{
	if (toSpawn)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = Cast<AFGSlimeCharacter>(GetOwner());
			FRotator rotator;
			FVector spawnLocation = Cast<AFGSlimeCharacter>(GetOwner())->GetActorLocation();
			world->SpawnActor<AFG_FollowerActor>(toSpawn, spawnLocation, rotator, spawnParams);
		}
	}
}


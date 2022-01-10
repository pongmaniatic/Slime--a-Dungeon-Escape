#pragma once
#include "GameFramework/Actor.h"
#include "FGLevelModule.generated.h"

UCLASS()
class SLIMEPLATFORMER_API AFGLevelModule : public AActor
{
	GENERATED_BODY()

public:
	AFGLevelModule();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	/** The index of the 'grid point' where the door exists, with 0 being either the top or the left depending on which side. Sides: Up, Down, Left, Right */
	UPROPERTY(EditDefaultsOnly, Category = "Corridor Index")
	TArray<int> DoorLocations = {-1, -1, -1, -1};

	UActorComponent* AddComponent(TSubclassOf<UActorComponent> Class, FName ComponentName);
};
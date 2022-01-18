#pragma once
#include "GameFramework/Actor.h"
#include "FGExitDoor.generated.h"

class UBoxComponent;
UCLASS()
class AFGExitDoor : public AActor
{
	GENERATED_BODY()

public:
	AFGExitDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;
};
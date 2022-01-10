#include "FGExitDoor.h"

#include "Components/BoxComponent.h"

AFGExitDoor::AFGExitDoor()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	BoxComponent->SetupAttachment(RootComponent);
}

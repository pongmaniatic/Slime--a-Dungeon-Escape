#pragma once
#include "Components/StaticMeshComponent.h"
#include "FGModuleWall.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SLIMEPLATFORMER_API UFGModuleWall : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UFGModuleWall();
};
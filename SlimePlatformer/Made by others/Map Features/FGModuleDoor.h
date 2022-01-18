#pragma once
#include "FGModuleFeature.h"
#include "FGModuleDoor.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SLIMEPLATFORMER_API UFGModuleDoor : public UFGModuleFeature
{
	GENERATED_BODY()

public:
	UFGModuleDoor();
};
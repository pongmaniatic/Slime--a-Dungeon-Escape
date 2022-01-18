#pragma once
#include "FGModuleFeature.h"
#include "FGModuleObstacle.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SLIMEPLATFORMER_API UFGModuleObstacle : public UFGModuleFeature
{
	GENERATED_BODY()

	public:
	UFGModuleObstacle();
};
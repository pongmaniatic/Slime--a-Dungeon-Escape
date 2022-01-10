#pragma once
#include "FGModuleFeature.h"
#include "FGModuleEnemy.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SLIMEPLATFORMER_API UFGModuleEnemy : public UFGModuleFeature
{
	GENERATED_BODY()

	public:
	UFGModuleEnemy();
};
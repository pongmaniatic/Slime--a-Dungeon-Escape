#pragma once
#include "FGModuleFeature.h"
#include "FGModuleItem.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UFGModuleItem : public UFGModuleFeature
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float PercentageOfBeingACoin;

	UPROPERTY(EditAnywhere)
	float PercentageOfBeginHealth;
};
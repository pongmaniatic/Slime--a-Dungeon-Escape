#pragma once
#include "Components/BillboardComponent.h"
#include "FGModuleFeature.generated.h"

UCLASS(Abstract)
class SLIMEPLATFORMER_API UFGModuleFeature : public UBillboardComponent
{
	GENERATED_BODY()

public:
	UFGModuleFeature();
};
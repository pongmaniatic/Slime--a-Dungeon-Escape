#pragma once
#include "Components/ActorComponent.h"
#include "FGPlayerProximityJumpComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerProximityJumpComponent, Log, All);

class AFGEnemy;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UFGPlayerProximityJumpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFGPlayerProximityJumpComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool DrawDebugLines = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range = 200.f;

	/** If the vector pointing from enemy to player has a lower dot product with UpVector then the player will be in the correct zone */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LowestDotProduct = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator EnemyRotationToFacePlayer;

private:
	UPROPERTY(Transient)
	AFGEnemy* Enemy;
};
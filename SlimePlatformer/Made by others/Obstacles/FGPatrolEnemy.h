#pragma once

#include "FGEnemy.h"
#include "FGPatrolEnemy.generated.h"

UCLASS()
class AFGPatrolEnemy : public AFGEnemy
{
	GENERATED_BODY()

public:
	AFGPatrolEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LineTraceLength = 50.f;

	/** The trace channel to be used to determine if the patrol enemy should turn and walk in the other direction (when it hits something in front of it, it turns) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> TurnTraceChannel;

private:
	void LineTraceCheck();
};
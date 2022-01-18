#pragma once
#include "FGEnemy.h"
#include "FGFlyingEnemy.generated.h"

UCLASS()
class AFGFlyingEnemy : public AFGEnemy
{
	GENERATED_BODY()

public:
	AFGFlyingEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** If player is within this distance, it will start flying towards it */
	UPROPERTY(EditAnywhere)
	float Range = 500.f;

	UPROPERTY(EditAnywhere)
	float MarginBeforeTurning = 50.f;
	
	UPROPERTY(EditAnywhere)
	float StartPositionUpOffset = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> PatrolTraceChannel;
	
	UPROPERTY(EditAnywhere)
	float Speed = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsRunningAway = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPlayerDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackCooldown = 0.5f;

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackPlayer();
	
private:
	bool IsPlayerSeen();
	void Patrol(float DeltaSeconds);
	void AttackPlayer(float DeltaSeconds);

	bool IsAttacking = false;
	float TimeWhenStartAttacking = -100.f;

	float TimeOfLastAttack = -100.f;
};

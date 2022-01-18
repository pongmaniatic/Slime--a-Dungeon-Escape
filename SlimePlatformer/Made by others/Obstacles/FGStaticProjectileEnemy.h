#pragma once
#include "FGEnemy.h"
#include "FGStaticProjectileEnemy.generated.h"

class AFGProjectile;
DECLARE_LOG_CATEGORY_EXTERN(LogStaticProjectileEnemy, Log, All);

class AFGSlimeCharacter;
UCLASS()
class AFGStaticProjectileEnemy : public AFGEnemy
{
	GENERATED_BODY()

public:
	AFGStaticProjectileEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Cooldown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFGProjectile> ProjectileClass;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnSpawnProjectile(AFGProjectile* Projectile);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool PlayerIsSeen = false;

private:
	float LastShot = -100.f;
};
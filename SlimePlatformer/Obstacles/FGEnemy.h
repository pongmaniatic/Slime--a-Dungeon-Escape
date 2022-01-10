#pragma once
#include "GameFramework/Character.h"
#include "FGEnemy.generated.h"

class AFGSlimeCharacter;
class UNiagaraSystem;

DECLARE_LOG_CATEGORY_CLASS(LogEnemy, Log, All);

UCLASS(Abstract)
class AFGEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AFGEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector LookDirection;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRotator LookRotator;

	/** This rotation will either face exactly left or right depending on the player's location */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRotator AxisAlignedLookRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool UseStandardHitLogic = true;

	UFUNCTION(BlueprintCallable)
	void SetLookDirection(FVector Direction);

	UFUNCTION(BlueprintCallable)
	AFGSlimeCharacter* GetPlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 1.f;

	/** The actual delay in seconds before this actor is destroyed, allows for playing death animations etc. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DeathDelay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimationAsset* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* DeathNiagaraSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDying = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDeath();

	UFUNCTION(BlueprintNativeEvent)
	void OnPlayerHit(UPrimitiveComponent* HitComponent, AFGSlimeCharacter* SlimeCharacter, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	AFGSlimeCharacter* PlayerCharacter;

	void OnDeath();
};

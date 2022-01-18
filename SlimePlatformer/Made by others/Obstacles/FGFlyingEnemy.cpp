#include "FGFlyingEnemy.h"

#include "Components/CapsuleComponent.h"
#include "SlimePlatformer/FGSlimeCharacter.h"

AFGFlyingEnemy::AFGFlyingEnemy()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void AFGFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();

	AddActorWorldOffset(StartPositionUpOffset * FVector::UpVector, true);
	SetLookDirection(FVector::RightVector * (FMath::RandBool() ? 1 : -1));
}

void AFGFlyingEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsRunningAway)
	{
		if(IsPlayerSeen())
			AttackPlayer(DeltaSeconds);
		else
			Patrol(DeltaSeconds);
	}

	if(!IsAttacking || GetGameTimeSinceCreation() - TimeWhenStartAttacking > AttackPlayerDelay)
		AddMovementInput(LookDirection, Speed);
}

bool AFGFlyingEnemy::IsPlayerSeen()
{
	const FVector Pos = GetActorLocation();
	const AFGSlimeCharacter* SlimeCharacter = GetPlayerCharacter();
	if(SlimeCharacter == nullptr)
		return false;
	
	const FVector PlayerPos = SlimeCharacter->GetActorLocation();
	
	const float DistanceSqr = FVector::DistSquared(Pos, PlayerPos);
	
	FHitResult Hit;
	return DistanceSqr < FMath::Square(Range) && !GetWorld()->LineTraceSingleByObjectType(Hit, Pos, PlayerPos,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllStaticObjects));
}

void AFGFlyingEnemy::AttackPlayer(float DeltaSeconds)
{
	const float Time = GetGameTimeSinceCreation();
	if(!IsAttacking)
	{
		IsAttacking = true;
		TimeWhenStartAttacking = Time;
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
	
	AFGSlimeCharacter* SlimeCharacter = GetPlayerCharacter();
	if(SlimeCharacter == nullptr)
		return;
	
	if(Time - TimeOfLastAttack > AttackCooldown && FVector::DistSquared(GetActorLocation(), SlimeCharacter->GetActorLocation()) < FMath::Square(AttackDistance))
	{
		TimeOfLastAttack = Time;
		OnAttackPlayer();
	}
	
	SetLookDirection((SlimeCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal());
}

void AFGFlyingEnemy::Patrol(float DeltaSeconds)
{
	if(IsAttacking)
	{
		SetLookDirection(FVector::RightVector * (FMath::RandBool() ? 1 : -1));
		IsAttacking = false;
	}

	const FVector Pos = GetActorLocation();

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	FCollisionShape Shape;
	Shape.SetCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight());
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	FHitResult Hit;
	GetWorld()->SweepSingleByChannel(Hit, Pos, Pos + LookDirection * MarginBeforeTurning, FQuat::Identity, PatrolTraceChannel, Shape, Params);

	if(Hit.bBlockingHit && !IsAttacking)
		SetLookDirection(-LookDirection);
}

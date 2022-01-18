#include "FGPatrolEnemy.h"

#include "Components/CapsuleComponent.h"

AFGPatrolEnemy::AFGPatrolEnemy()
{
	
}

void AFGPatrolEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetLookDirection(FVector::RightVector * (FMath::RandBool() ? 1 : -1));
}

void AFGPatrolEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LineTraceCheck();

	AddMovementInput(LookDirection);
}

void AFGPatrolEnemy::LineTraceCheck()
{
	const FVector Pos = GetActorLocation();
	const FVector PosInFront = Pos + LookDirection * LineTraceLength;
	
	FCollisionQueryParams QueryResponse;
	QueryResponse.AddIgnoredActor(this);

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	FCollisionShape Shape;
	Shape.SetCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight());
	
	FHitResult Hit;
	const bool WallIsInFront = GetWorld()->SweepSingleByChannel(Hit, Pos, PosInFront, FQuat::Identity,
		TurnTraceChannel, Shape, QueryResponse);
	
	const bool LedgeIsInFront = !GetWorld()->LineTraceSingleByObjectType(Hit, PosInFront,
		PosInFront + FVector::DownVector * LineTraceLength,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllStaticObjects));

	if(WallIsInFront || LedgeIsInFront)
		SetLookDirection(-LookDirection);
}

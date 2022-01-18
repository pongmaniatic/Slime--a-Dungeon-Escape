#include "FGPlayerProximityJumpComponent.h"

#include "FGEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlimePlatformer/FGSlimeCharacter.h"

DEFINE_LOG_CATEGORY(LogPlayerProximityJumpComponent);

UFGPlayerProximityJumpComponent::UFGPlayerProximityJumpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFGPlayerProximityJumpComponent::BeginPlay()
{
	Super::BeginPlay();

	Enemy = GetOwner<AFGEnemy>();
	if(Enemy == nullptr)
	{
		UE_LOG(LogPlayerProximityJumpComponent, Error, TEXT("Player proximity component is attached to a non-enemy, the proximity component will do nothing"));
	}
}

void UFGPlayerProximityJumpComponent::TickComponent(float DeltaTime, ELevelTick Tick, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	const FVector Pos = Enemy->GetActorLocation();
	const AFGSlimeCharacter* SlimeCharacter = Enemy->GetPlayerCharacter();
	
	if(SlimeCharacter == nullptr)
		return;

	const FVector PlayerPos = SlimeCharacter->GetActorLocation();
	const FVector VectorPointingToPlayer = (PlayerPos - Pos).GetSafeNormal();

	const float Dot = FVector::DotProduct(VectorPointingToPlayer, FVector::UpVector);

	if(DrawDebugLines)
	{
		constexpr float Tau = 6.28318530717959f;
		constexpr float Deg2Rad = Tau / 360.0f;
		
		const float AngleDeg = FMath::Lerp(0, 90, LowestDotProduct);
		const float AngleRadRight = AngleDeg * Deg2Rad;
		const float AngleRadLeft = Tau * 0.5f - AngleRadRight;

		FVector LineRight = FVector(0.f, FMath::Cos(AngleRadRight), FMath::Sin(AngleRadRight));
		FVector LineLeft = FVector(0.f, FMath::Cos(AngleRadLeft), FMath::Sin(AngleRadLeft));
		
		UKismetSystemLibrary::DrawDebugLine(this, Pos, Pos + LineLeft * Range, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(this, Pos, Pos + LineRight * Range, FLinearColor::Red);
	}

	FHitResult Hit;
	const bool PlayerIsNotBehindWall = !GetWorld()->LineTraceSingleByObjectType(Hit, Pos, PlayerPos, FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllStaticObjects));

	if(Dot > LowestDotProduct && FVector::DistSquared(Pos, PlayerPos) < FMath::Square(Range) && PlayerIsNotBehindWall)
	{
		Enemy->Jump();
	}
}

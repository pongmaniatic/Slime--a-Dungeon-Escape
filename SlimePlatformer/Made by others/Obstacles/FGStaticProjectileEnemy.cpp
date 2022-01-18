#include "FGStaticProjectileEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SlimePlatformer/FGSlimeCharacter.h"
#include "SlimePlatformer/Weapons/FGProjectile.h"

DEFINE_LOG_CATEGORY(LogStaticProjectileEnemy);

AFGStaticProjectileEnemy::AFGStaticProjectileEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFGStaticProjectileEnemy::BeginPlay()
{
	Super::BeginPlay();

	FHitResult RightHit;
	FHitResult LeftHit;
	FVector Position = GetActorLocation();
	GetWorld()->LineTraceSingleByObjectType(RightHit, Position, Position + FVector::RightVector * 10000.f, FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllStaticObjects));
	GetWorld()->LineTraceSingleByObjectType(LeftHit, Position, Position + FVector::LeftVector * 10000.f, FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllStaticObjects));

	if(LeftHit.bBlockingHit && RightHit.bBlockingHit)
	{
		if(LeftHit.Time < RightHit.Time)
		{
			SetActorRotation(UKismetMathLibrary::MakeRotFromX(FVector::RightVector));
		}
		else
		{
			SetActorRotation(UKismetMathLibrary::MakeRotFromX(FVector::LeftVector));
		}
		return;
	}
	
	if(LeftHit.bBlockingHit)
	{
		SetActorRotation(UKismetMathLibrary::MakeRotFromX(FVector::RightVector));
		return;
	}

	if(RightHit.bBlockingHit)
	{
		SetActorRotation(UKismetMathLibrary::MakeRotFromX(FVector::LeftVector));
		return;
	}

	SetActorRotation(UKismetMathLibrary::MakeRotFromX(FVector::RightVector));
}

void AFGStaticProjectileEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AFGSlimeCharacter* SlimeCharacter = GetPlayerCharacter();

	if(SlimeCharacter == nullptr || IsDying)
		return;

	const float Time = GetGameTimeSinceCreation();
	const FVector Pos = GetActorLocation();
	const FVector PlayerPos = SlimeCharacter->GetActorLocation();
	
	if(FVector::DistSquared(PlayerPos, Pos) <= FMath::Square(Range))
	{
		FCollisionQueryParams CollisionParams;
		CollisionParams.TraceTag = TEXT("Projectile Enemy");
		
		
		FHitResult HitResult;
		if(!GetWorld()->LineTraceSingleByObjectType(HitResult, Pos, PlayerPos, FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllStaticObjects), CollisionParams))
		{
			PlayerIsSeen = true;
			SetLookDirection((PlayerPos - Pos).GetSafeNormal());

			if(Time - LastShot > Cooldown)
			{
				LastShot = Time;
				
				AFGProjectile* Projectile = GetWorld()->SpawnActor<AFGProjectile>(ProjectileClass);
				Projectile->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
				Projectile->SetActorLocation(Pos);
				Projectile->SetActorRotation(LookRotator);
				BP_OnSpawnProjectile(Projectile);
			}
		}
		else
		{
			PlayerIsSeen = false;
		}
	}
}

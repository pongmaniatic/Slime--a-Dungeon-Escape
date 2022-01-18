#include "FGEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "SlimePlatformer/FGSlimeCharacter.h"

AFGEnemy::AFGEnemy()
{
	SetCanBeDamaged(true);
}

void AFGEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(Controller == nullptr)
		SpawnDefaultController();

	PlayerCharacter = Cast<AFGSlimeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(PlayerCharacter == nullptr)
		UE_LOG(LogEnemy, Error, TEXT("Couldn't get player pawn"));

	OnTakePointDamage.AddDynamic(this, &AFGEnemy::OnTakeDamage);
	
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFGEnemy::OnCapsuleHit);
}

void AFGEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector Pos = GetActorLocation();
	SetActorLocation(FVector(0.f, Pos.Y, Pos.Z));

	AFGSlimeCharacter* SlimeCharacter = GetPlayerCharacter();
	if(SlimeCharacter == nullptr)
		return;
	
	const FVector PlayerPos = SlimeCharacter->GetActorLocation();
	const FVector VectorPointingToPlayer = (PlayerPos - Pos).GetSafeNormal();
	const float DotBetweenRightAndVector = FVector::DotProduct(VectorPointingToPlayer, FVector::RightVector);

	if(DotBetweenRightAndVector < 0)
	{
		AxisAlignedLookRotator = UKismetMathLibrary::MakeRotFromX(-FVector::RightVector);
	}
	else
	{
		AxisAlignedLookRotator = UKismetMathLibrary::MakeRotFromX(FVector::RightVector);
	}
}

void AFGEnemy::SetLookDirection(FVector Direction)
{
	LookDirection = FVector(0.f, Direction.Y, Direction.Z).GetSafeNormal();
	LookRotator = UKismetMathLibrary::MakeRotFromX(LookDirection);
}

AFGSlimeCharacter* AFGEnemy::GetPlayerCharacter()
{
	if(PlayerCharacter != nullptr)
		return PlayerCharacter;

	PlayerCharacter = Cast<AFGSlimeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	return PlayerCharacter;
}

void AFGEnemy::OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                            UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
                            AActor* DamageCauser)
{
	Health -= Damage;
	
	if(Health <= 0)
	{
		OnDeath();
	}
}

void AFGEnemy::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AFGSlimeCharacter* Character = Cast<AFGSlimeCharacter>(OtherActor);

	if(Character == nullptr || IsDying)
		return;

	OnPlayerHit(HitComponent, Character, OtherComp, NormalImpulse, Hit);
}

void AFGEnemy::OnDeath()
{
	IsDying = true;
	BP_OnDeath();

	if(FMath::IsNearlyZero(DeathDelay))
	{
		Destroy();
		return;
	}

	if(DeathAnimation != nullptr)
		GetMesh()->PlayAnimation(DeathAnimation, false);

	if(DeathParticle != nullptr)
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());

	if(DeathNiagaraSystem != nullptr)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathNiagaraSystem, GetActorLocation());
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
	{
		Destroy();
	}, DeathDelay, false);
}

void AFGEnemy::OnPlayerHit_Implementation(UPrimitiveComponent* HitComponent, AFGSlimeCharacter* SlimeCharacter,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(SlimeCharacter, 1.f, Controller, this, UDamageType::StaticClass());
}

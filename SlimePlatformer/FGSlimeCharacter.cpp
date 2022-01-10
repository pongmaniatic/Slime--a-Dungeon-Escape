#include "FGSlimeCharacter.h"
#include "Components/BoxComponent.h"
#include "Interface/FGItemInterface.h"

#include "Systems/Followers/FG_FollowerActor.h"

#include "Interface/FGItem.h"
#include <iostream>

AFGSlimeCharacter::AFGSlimeCharacter()
{
 	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	// Creates the main mesh and locks its rotation
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<BaseMesh>"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetUsingAbsoluteRotation(true);

	CameraHolder = CreateDefaultSubobject<USpringArmComponent>(TEXT("<USpringArmComponent>"));
	CameraHolder->SetupAttachment(RootComponent);
	CameraHolder->bEnableCameraLag = true;
	CameraHolder->bUseCameraLagSubstepping = true;
	CameraHolder->CameraLagMaxDistance = cameraLagMaxDistance;
	CameraHolder->CameraLagSpeed = cameraLagSpeed;


	// Don't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Create Camera and also locks rotation
	CentralCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CentralCamera->SetupAttachment(CameraHolder);
	CentralCamera->SetUsingAbsoluteRotation(true);


	collidingOnRightSide = false;
}

void AFGSlimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	PlayerInputComponent->BindAction("DiscardAction", IE_Pressed, this, &AFGSlimeCharacter::DiscardAction);
	PlayerInputComponent->BindAction("ActiveAction", IE_Pressed, this, &AFGSlimeCharacter::ActiveAction);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFGSlimeCharacter::StopSprint);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFGSlimeCharacter::PassiveAction);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFGSlimeCharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFGSlimeCharacter::Sprint);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFGSlimeCharacter::Jump);
	PlayerInputComponent->BindAxis("VerticalMove", this, &AFGSlimeCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFGSlimeCharacter::MoveRight);

	
}

void AFGSlimeCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Configure character movement
	GetCharacterMovement()->AirControl = airControl;
	GetCharacterMovement()->GroundFriction = groundFriction;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void AFGSlimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckState();

	UpdateCurrentState();
	
	CoyoteJumpCheck(DeltaTime);

	CheckForForgivenessJump(DeltaTime);

	CheckInvulnerable();

	CheckStunned();
}

void AFGSlimeCharacter::CheckState()
{
	// Check what state the slime should be in
	if (GetCharacterMovement()->IsMovingOnGround()){CurrentState = 1;}
	else 
	{
		/*
		if (collidingOnLeftSide > 0 || collidingOnRightSide > 0 && CurrentSlimeDirection <= 0 || CurrentSlimeDirection >= 0)
		{
			if (CurrentState == 2 && GetCharacterMovement()->Velocity.Z < 0) { GetCharacterMovement()->Velocity = FVector::ZeroVector; }
			CurrentState = 3;
		}
		*/

		if (collidingOnLeftSide > 0 && CurrentSlimeDirection <= 0)
		{ 
			if (CurrentState == 2 && GetCharacterMovement()->Velocity.Z < 0){GetCharacterMovement()->Velocity = FVector::ZeroVector;}
			CurrentState = 3;
		}
		else if ( collidingOnRightSide > 0 && CurrentSlimeDirection >= 0) 
		{ 
			if (CurrentState == 2 && GetCharacterMovement()->Velocity.Z < 0){GetCharacterMovement()->Velocity = FVector::ZeroVector;}
			CurrentState = 3;
		}
		else { CurrentState = 2; }
	}
}

void AFGSlimeCharacter::CoyoteJumpCheck(float DeltaTime)
{
	if (CurrentState == 2) // if airborne
	{
		coyoteJumpTime += DeltaTime;
		if (coyoteJumpTime > coyoteJumpMaxTime) { slimeCanJump = false; }
	}
}

void AFGSlimeCharacter::CheckForForgivenessJump(float DeltaTime)
{
	if (IntendsToJump)
	{
		jumpMemoryTime += DeltaTime;
		if (jumpMemoryTime > jumpMemoryMaxTime)
		{
			IntendsToJump = false;
			jumpMemoryTime = 0.0f;
		}
		if (GetCharacterMovement()->IsMovingOnGround() && slimeCanJump){Jump();}
	}
}

void AFGSlimeCharacter::WallJumpCheck()
{
	if (CurrentState == 3) 
	{
		if (collidingOnLeftSide > 0)
		{
			OnJumped();
			slimeCanJump = false;
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->GravityScale = standardGravity;
			GetCharacterMovement()->AddImpulse(FVector(0.0f, 890.0f, jumpStrength), true);
		}
		else if (collidingOnRightSide > 0)
		{
			OnJumped();
			slimeCanJump = false;
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->GravityScale = standardGravity;
			GetCharacterMovement()->AddImpulse(FVector(0.0f, -890.0f, jumpStrength), true);
		}
	}

	else
	{
		if (LastState == 1)
		{
			OnJumped();
			slimeCanJump = false;
			GetCharacterMovement()->GravityScale = standardGravity;
			GetCharacterMovement()->AddImpulse(FVector(0.0f, 0.0f, jumpStrength), true);
		}
	
		if (LastState == 3)
		{
			if (collidingOnLeftSide > 0)
			{
				OnJumped();
				slimeCanJump = false;
				GetCharacterMovement()->Velocity = FVector::ZeroVector;
				GetCharacterMovement()->GravityScale = standardGravity;
				GetCharacterMovement()->AddImpulse(FVector(0.0f, 890.0f, jumpStrength ), true);
			}
			else if (collidingOnRightSide > 0)
			{
				OnJumped();
				slimeCanJump = false;
				GetCharacterMovement()->Velocity = FVector::ZeroVector;
				GetCharacterMovement()->GravityScale = standardGravity;
				GetCharacterMovement()->AddImpulse(FVector(0.0f, -890.0f, jumpStrength ), true);
			}
		}
	}
}

void AFGSlimeCharacter::Jump()
{
	if (slimeCanJump){WallJumpCheck();}
	else{ IntendsToJump = true;  }
}

void AFGSlimeCharacter::StopJumping()
{
	if (GetCharacterMovement()->Velocity.Z > 0){GetCharacterMovement()->Velocity.Z = GetCharacterMovement()->Velocity.Z / jumpCancelSpeedDivision;}
}

void AFGSlimeCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
	sprinting = true;
}

void AFGSlimeCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	sprinting = false;
}

void AFGSlimeCharacter::UpdateCurrentState()
{
	// current state: 
	// 1 grounded
	// 2 airborne
	// 3 wallcling
	switch (CurrentState)
	{
	case 1:
	{
		LastState = 1;
		coyoteJumpTime = 0.0f;
		slimeCanJump = true;
		GetCharacterMovement()->GravityScale = standardGravity;
		break;
	}
	case 2:
	{
		GetCharacterMovement()->GravityScale = standardGravity;
		break;
	}
	case 3:
	{
		LastState = 3;
		coyoteJumpTime = 0.0f;
		slimeCanJump = true;
		if (GetCharacterMovement()->Velocity.Z < 0 ) { GetCharacterMovement()->GravityScale = 0.5f; }
		break;
	}
	default: break;
	}

}

void AFGSlimeCharacter::ActiveAction()
{
	InterfaceActiveAbility();
}

void AFGSlimeCharacter::PassiveAction()
{
	InterfacePassiveAbility();
}

void AFGSlimeCharacter::DiscardAction()
{
	InterfaceDiscardAbility();
}

void AFGSlimeCharacter::ActivateUpgrade()
{
	InterfaceStatUpgrade();
}





void AFGSlimeCharacter::MoveRight(float Value)
{
	CurrentSlimeDirection = Value;
	// This moves the character left and right given the input.
	if (!stunned)
	{
	AddMovementInput(FVector(0.f, movementSpeed, 0.f), Value*2.5);
	}
}

void AFGSlimeCharacter::MoveUp(float Value)
{
	CurrentSlimeDirectionUpDown = Value;
}

void AFGSlimeCharacter::CheckInvulnerable()
{
	if (invulnerable)
	{
		if (invulnerableTimer <= 0){invulnerableTimer = invulnerableTimerMax;}
		invulnerableTimer -= GetWorld()->GetDeltaSeconds();
		if (invulnerableTimer <= 0){invulnerable = false;}
	}
}

void AFGSlimeCharacter::CheckStunned()
{
	if (stunned)
	{
		if (stunnedTimer <= 0){ stunnedTimer = stunnedTimerMax;}
		stunnedTimer -= GetWorld()->GetDeltaSeconds();
		if (stunnedTimer <= 0){stunned = false;}
	}
}

void AFGSlimeCharacter::ReCreateItem()
{
	if (toReCreateItem)
	{
		UWorld* world = GetWorld();
		if (world)
		{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FRotator rotator;
		FVector spawnLocation = this->GetActorLocation();
		heldItem = world->SpawnActor<AActor>(toReCreateItem, spawnLocation, rotator, spawnParams);
		AFGItem* heldItemCasted = Cast<AFGItem>(heldItem);
		heldItemCasted->currentlyBeingHeld = true;
		heldItemCasted->slimePlayer = this;
		heldItemCasted->ReSetItem();
		}
	}
}

void AFGSlimeCharacter::ReCreateUpgrades()
{
	int numbOfUpgrades = 0;
	for (UClass* upgrade : toReCreateUpgrades)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			if(upgrade == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Current upgrade class is null (ReCreateUpgrades() in FGSlimeCharacter.cpp)"));
				continue;
			}
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			FRotator rotator;
			FVector spawnLocation = this->GetActorLocation();
			AActor* currentUpgrade = world->SpawnActor<AActor>(upgrade, spawnLocation, rotator, spawnParams);
			if(currentUpgrade == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Current upgrade is null (ReCreateUpgrades() in FGSlimeCharacter.cpp)"));
				continue;
			}
			AFG_FollowerActor* currentUpgradeCasted = Cast<AFG_FollowerActor>(currentUpgrade);
			if(currentUpgradeCasted == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Current upgrade failed casting (ReCreateUpgrades() in FGSlimeCharacter.cpp)"));
				continue;
			}
			currentUpgradeCasted->slimeCharacter = this;
			currentUpgradeCasted->position = numbOfUpgrades;
			currentUpgradeCasted->ReSetFollower();
			currentUpgradeCasted->bought = true;
			currentUpgrades.Add(currentUpgrade);
			
		}
		numbOfUpgrades += 1;
	}
}




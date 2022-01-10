// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ActorComponent.h"


#include "FGSlimeCharacter.generated.h"

UCLASS()
class SLIMEPLATFORMER_API AFGSlimeCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CentralCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	// Set default values
	AFGSlimeCharacter();

	// Move right and left
	void MoveRight(float Val);

	void MoveUp(float Val);

	void Jump();

	void WallJumpCheck();

	void StopJumping();

	void Sprint();

	void StopSprint();

	void CheckForForgivenessJump(float DeltaTime);

	void CheckState();

	void CoyoteJumpCheck(float DeltaTime);

	void UpdateCurrentState();

	void ActiveAction();

	void PassiveAction();

	void DiscardAction();

	void ActivateUpgrade();

	void CheckInvulnerable();

	void CheckStunned();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;


	UFUNCTION(BlueprintImplementableEvent)
		void InterfaceActiveAbility();

	UFUNCTION(BlueprintImplementableEvent)
		void InterfacePassiveAbility();

	UFUNCTION(BlueprintImplementableEvent)
		void InterfaceDiscardAbility();

	UFUNCTION(BlueprintImplementableEvent)
		void InterfaceStatUpgrade();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void KnockBack(FVector hitPoint);

	UFUNCTION(BlueprintCallable)
		void ReCreateItem();

	UFUNCTION(BlueprintCallable)
		void ReCreateUpgrades();


	// Jump related variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float jumpStrength = 1500.0f;

	UPROPERTY(EditAnywhere, Category = Jump)
	float coyoteJumpMaxTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = Jump)
	float jumpCancelSpeedDivision = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = Jump)
	float coyoteJumpTime = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Jump)
	bool slimeCanJump = false;

	UPROPERTY(VisibleAnywhere, Category = Jump)
	bool IntendsToJump = false;

	UPROPERTY(VisibleAnywhere, Category = Jump)
	float jumpMemoryTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = Jump)
	float jumpMemoryMaxTime = 2.0f;

	//Movement related variables
	UPROPERTY(VisibleAnywhere, Category = Movement)
	bool sprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float walkSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float sprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float groundFriction = 0.5f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float movementSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float airControl = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	bool airBorne = false;

	UPROPERTY(BlueprintReadWrite, Category = Collision)
	int collidingOnLeftSide = 0;

	UPROPERTY(BlueprintReadWrite, Category = Collision)
	int collidingOnRightSide = 0;

	UPROPERTY(BlueprintReadWrite, Category = State)
	int CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentSlimeDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentSlimeDirectionUpDown;

	//Other variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Other)
	float fallGravMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Other)
	float standardGravity = 3.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Other)
	int LastState = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Other)
	AActor* heldItem;
	
	UPROPERTY(Transient)
	UClass* toReCreateItem;

	UPROPERTY(Transient)
	TArray<UClass*> toReCreateUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Other)
	TArray<AActor*> currentUpgrades;


	//Camera related things
	UPROPERTY(BlueprintReadWrite, Category = Camera)
	USpringArmComponent* CameraHolder;

	UPROPERTY(BlueprintReadWrite, Category = Camera)
	float cameraLagMaxDistance = 300.0f;

	UPROPERTY(BlueprintReadWrite, Category = Camera)
	float cameraLagSpeed = 0.5f;


	//Invulnerable, life and damage related things
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	bool invulnerable = false;

	UPROPERTY(VisibleAnywhere)
	float invulnerableTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float invulnerableTimerMax = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool stunned = false;

	UPROPERTY(VisibleAnywhere)
	float stunnedTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float stunnedTimerMax = 0.5f;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

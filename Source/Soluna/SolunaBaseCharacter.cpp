// Fill out your copyright notice in the Description page of Project Settings.


#include "SolunaBaseCharacter.h"
#include "GameFramework/Controller.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASolunaBaseCharacter::ASolunaBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Add the AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ASCReplicationMode);

	// Add the BasicAttributeSet
	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

}

void ASolunaBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ASolunaBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystemComponent) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}


// Called when the game starts or when spawned
void ASolunaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent) {
		AbilitySystemComponent->AbilityFailedCallbacks.AddUObject(this, &ASolunaBaseCharacter::OnAbilityFailed);
	}
	
}

void ASolunaBaseCharacter::OnAbilityFailed(
	const UGameplayAbility* Ability,
	const FGameplayTagContainer& FailureTags)
{

	for (auto Tag : FailureTags)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failure Tag: %s"), *Tag.ToString());
	}
	if (FailureTags.HasTagExact(
		FGameplayTag::RequestGameplayTag("Ability.ActivateFail.CantAffordCost")))
	{
		BP_OnAbilityFailed(FailureTags);
	}
}

// Called every frame
void ASolunaBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraAngle = FMath::FInterpConstantTo(
		CameraAngle,
		TargetCameraAngle,
		DeltaTime,
		250.f   // degrees per second
	);
}

UAbilitySystemComponent* ASolunaBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FRotator ASolunaBaseCharacter::GetViewRotation() const
{
	if (Controller)
	{
		FRotator Rot = Controller->GetControlRotation();
		Rot.Yaw += CameraAngle;
		return Rot;
	}

	return Super::GetViewRotation();

}

void ASolunaBaseCharacter::OrientRotationToMovement_Implementation()
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}

}

void ASolunaBaseCharacter::OrientRotationToController_Implementation()
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void ASolunaBaseCharacter::RotateCamera_Implementation(float TargetYawAngle)
{
	TargetCameraAngle = TargetYawAngle;
}


void ASolunaBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASolunaBaseCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASolunaBaseCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASolunaBaseCharacter::Look);
	}
}

void ASolunaBaseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ASolunaBaseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ASolunaBaseCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetViewRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ASolunaBaseCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ASolunaBaseCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ASolunaBaseCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}


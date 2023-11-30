// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Controller.h"
#include "Item/Weapon.h"
#include "Kismet/KismetMathLibrary.h"

ACHeroCharacter::ACHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ACHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Move);
		EnhancedInputComponent->BindAction(EquipKeyAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Equip);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Attack);
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ACHeroCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ActorsToIgnore.Empty();
	}
}

void ACHeroCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector =  Value.Get<FVector2D>().GetRotated(-45.f);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		return;
	}
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red,
			                                 FString::Printf(
				                                 TEXT("movement x: %f, y: %f"), MovementVector.X, MovementVector.Y));
		}

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACHeroCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquipOneHandedSword;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
}

void ACHeroCharacter::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)
	{
		if (AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			AttackState = 1;
			return;
		}
		AnimInstance->Montage_Play(AttackMontage);
		FName SectionName = FName();

		switch (AttackState)
		{
		case 1:
			SectionName = "SwordSlash1";
			AttackState = 1;
			break;
		// case 2:
		// 	SectionName = "SwordSlash2";
		// 	AttackState = 1;
		// 	break;
		// case 3:
		// 	SectionName = "SwordSlash3";
		// 	AttackState = 1;
		// 	break;
		default:
			SectionName = "SwordSlash1";
			AttackState = 1;
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

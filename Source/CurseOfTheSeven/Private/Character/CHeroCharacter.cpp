// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "LegacyCameraShake.h"
#include "Character/SkillComponent/SkillSlotComponent.h"
#include "Components/BoxComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/Controller.h"
#include "Item/Weapon.h"
#include "Utility/AnimationComponent.h"

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

	AnimationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComponent"));
	
	FirstSkillSlotComponent = CreateDefaultSubobject<USkillSlotComponent>(TEXT("FirstSkillSlot"));
	SecondSkillSlotComponent = CreateDefaultSubobject<USkillSlotComponent>(TEXT("SecondSkillSlot"));
	UltimateSkillSlotComponent = CreateDefaultSubobject<USkillSlotComponent>(TEXT("UltimateSkillSlot"));
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

void ACHeroCharacter::ShakeCamera()
{
	GetLocalViewingPlayerController()->ClientStartCameraShake(CameraShake);
}

void ACHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Move);
		EnhancedInputComponent->BindAction(EquipKeyAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Equip);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Dash);
		EnhancedInputComponent->BindAction(FirstSkillAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::CastFirstSkill);
		EnhancedInputComponent->BindAction(SecondSkillAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::CastSecondSkill);
		EnhancedInputComponent->BindAction(UltimateSkillAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::CastUltimateSkill);
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
	MovementVector =  Value.Get<FVector2D>().GetRotated(-45.f);
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

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACHeroCharacter::Dash()
{
	AnimationComponent->Dash(this, FVector(MovementVector.Y * 500, MovementVector.X * 500, 0.f));
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
		OverlappingWeapon->OnWeaponHit.BindUObject(this, &ACHeroCharacter::ShakeCamera);
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

void ACHeroCharacter::CastFirstSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Montage_IsPlaying(FirstSkillSlotComponent->GetSkillMontage()))
	{
		return;
	}
	AnimInstance->Montage_Play(FirstSkillSlotComponent->GetSkillMontage());

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::SpawnFirstSkill, FirstSkillSlotComponent->GetDelay(), false);
	
}

void ACHeroCharacter::SpawnFirstSkill()
{
	FirstSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation());
}

void ACHeroCharacter::CastSecondSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Montage_IsPlaying(SecondSkillSlotComponent->GetSkillMontage()))
	{
		return;
	}
	AnimInstance->Montage_Play(SecondSkillSlotComponent->GetSkillMontage());

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::SpawnSecondSkill, FirstSkillSlotComponent->GetDelay(), false);
}

void ACHeroCharacter::SpawnSecondSkill()
{
	SecondSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation());
}

void ACHeroCharacter::CastUltimateSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Montage_IsPlaying(UltimateSkillSlotComponent->GetSkillMontage()))
	{
		return;
	}
	AnimInstance->Montage_Play(UltimateSkillSlotComponent->GetSkillMontage());

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::SpawnUltimateSkill, UltimateSkillSlotComponent->GetDelay(), false);
}

void ACHeroCharacter::SpawnUltimateSkill()
{
	UltimateSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation());
}



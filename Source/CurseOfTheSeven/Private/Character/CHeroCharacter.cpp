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
#include "NiagaraComponent.h"
#include "Character/SkillComponent/SkillSlotComponent.h"
#include "Components/BoxComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/Controller.h"
#include "Item/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Save/SaveSystem.h"
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
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
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

	DashVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashVfx"));
	DashVFX->SetupAttachment(RootComponent);

	AnimationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComponent"));

	FirstSkillSlotComponent = CreateDefaultSubobject<USkillSlotComponent>(TEXT("FirstSkillSlot"));
	SecondSkillSlotComponent = CreateDefaultSubobject<USkillSlotComponent>(TEXT("SecondSkillSlot"));
	UltimateSkillSlotComponent = CreateDefaultSubobject<USkillSlotComponent>(TEXT("UltimateSkillSlot"));

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ACHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDashing)
	{
		if (UKismetMathLibrary::Abs(GetVelocity().X) >= 3500.f)
		{
			DRAW_TEXT_ONSCREEN(GetVelocity().ToString());
			IsDashing = false;
		}
	}

	CheckAttack();
}

void ACHeroCharacter::SecondaryAttack()
{
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
	Tags.Add(FName("EngageableTarget"));
}

void ACHeroCharacter::ShakeCamera()
{
	GetLocalViewingPlayerController()->ClientStartCameraShake(CameraShake);
}

void ACHeroCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	// PlayHitSound(ImpactPoint);
	// SpawnHitParticles(ImpactPoint);
}

float ACHeroCharacter::GetMovementAngle()
{
	float Angle = 0.f;
	float DotProduct = UKismetMathLibrary::DotProduct2D(MovementVector, FVector2D(1.f, 0.f));

	if (MovementVector.Y > 0)
	{
		Angle = UKismetMathLibrary::DegAcos(DotProduct);

		if (MovementVector.X < 0)
		{
			Angle += 180.f;
		}
	}
	else
	{
		Angle = 360 - UKismetMathLibrary::DegAcos(DotProduct);

		if (MovementVector.X > 0)
		{
			Angle += 180.f;
		}
	}
	DRAW_TEXT_ONSCREEN(FString::Printf(TEXT("%f"), Angle));
	return Angle;
}

void ACHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Move);
		EnhancedInputComponent->BindAction(EquipKeyAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Equip);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Dash);
		EnhancedInputComponent->BindAction(FirstSkillAction, ETriggerEvent::Triggered, this,
		                                   &ACHeroCharacter::CastFirstSkill);
		EnhancedInputComponent->BindAction(SecondSkillAction, ETriggerEvent::Triggered, this,
		                                   &ACHeroCharacter::CastSecondSkill);
		EnhancedInputComponent->BindAction(UltimateSkillAction, ETriggerEvent::Triggered, this,
		                                   &ACHeroCharacter::CastUltimateSkill);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACHeroCharacter::Attack);
		EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Triggered, this,
		                                   &ACHeroCharacter::SecondaryAttack);
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ACHeroCharacter::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>().GetRotated(-45.f);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsDashing)
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
	SetActorRotation(FRotator(0.f, GetMovementAngle(), 0.f), ETeleportType::None);
	IsDashing = true;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::ResetDash,
	                                AnimationComponent->GetDashTime() - 0.05f, false);
	AnimationComponent->Dash(this, FVector(MovementVector.Y, MovementVector.X, 0.f).GetSafeNormal() * 300.f);
}

void ACHeroCharacter::ResetDash()
{
	IsDashing = false;
	IsAttacking = false;
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
	IsAttacking = true;
	if (AttackHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AttackHandle);
	}
	GetWorldTimerManager().SetTimer(AttackHandle, this, &ACHeroCharacter::ResetAttack,
	                                0.5f, false);
	//AnimInstance = GetMesh()->GetAnimInstance();

	// if (AnimInstance && AttackMontage)
	// {
	// 	if (AnimInstance->Montage_IsPlaying(AttackMontage))
	// 	{
	// 		AttackState = 1;
	// 		return;
	// 	}
	// 	AnimInstance->Montage_Play(AttackMontage);
	// 	FName SectionName = FName();
	//
	// 	switch (AttackState)
	// 	{
	// 	case 1:
	// 		SectionName = "SwordSlash1";
	// 		AttackState = 1;
	// 		break;
	// 	// case 2:
	// 	// 	SectionName = "SwordSlash2";
	// 	// 	AttackState = 1;
	// 	// 	break;
	// 	// case 3:
	// 	// 	SectionName = "SwordSlash3";
	// 	// 	AttackState = 1;
	// 	// 	break;
	// 	default:
	// 		SectionName = "SwordSlash1";
	// 		AttackState = 1;
	// 		break;
	// 	}
	// 	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	// }
}

void ACHeroCharacter::ResetAttack()
{
	IsAttacking = false;
	DRAW_TEXT_ONSCREEN(TEXT("ResetAttack"));
}


void ACHeroCharacter::SaveCharacterStatus()
{
	if (USaveSystem* SaveGameInstance = Cast<USaveSystem>(
		UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass())))
	{
		// Set up the (optional) delegate.
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		// USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
		SavedDelegate.BindUObject(this, &ACHeroCharacter::SaveDelegate);

		// Set data on the savegame object.
		SaveGameInstance->LevelName = GetLevel()->GetName();

		// Start async save process.
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, TEXT("Player_0"), 0, SavedDelegate);
	}
}

void ACHeroCharacter::LoadCharacterStatus()
{
}

void ACHeroCharacter::SaveDelegate(const FString& SlotName, int UserIndex, bool IsSuccess)
{
	FString result = IsSuccess ? TEXT("success") : TEXT("fail");
	DRAW_TEXT_ONSCREEN(result + " , " + SlotName + " , " + FString::FromInt(UserIndex));
}

void ACHeroCharacter::CastFirstSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(FirstSkillSlotComponent->GetSkillMontage()))
	{
		return;
	}
	AnimInstance->Montage_Play(FirstSkillSlotComponent->GetSkillMontage());

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::SpawnFirstSkill,
	                                FirstSkillSlotComponent->GetDelay(), false);
}

void ACHeroCharacter::SpawnFirstSkill()
{
	FirstSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation());
}

void ACHeroCharacter::CastSecondSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(SecondSkillSlotComponent->GetSkillMontage()))
	{
		return;
	}
	AnimInstance->Montage_Play(SecondSkillSlotComponent->GetSkillMontage());

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::SpawnSecondSkill,
	                                SecondSkillSlotComponent->GetDelay(), false);
}

void ACHeroCharacter::SpawnSecondSkill()
{
	SecondSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation());
}

void ACHeroCharacter::CastUltimateSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(UltimateSkillSlotComponent->GetSkillMontage()))
	{
		return;
	}
	AnimInstance->Montage_Play(UltimateSkillSlotComponent->GetSkillMontage());

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::SpawnUltimateSkill,
	                                UltimateSkillSlotComponent->GetDelay(), false);
}

void ACHeroCharacter::SpawnUltimateSkill()
{
	UltimateSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation());
}

void ACHeroCharacter::CheckAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	bool IsAnimPlaying = AnimInstance && (AnimInstance->Montage_IsPlaying(AttackMontage) || AnimInstance->
		Montage_IsPlaying(SpecialAttackMontage));
	if (AnimInstance && AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else if (!AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
}

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
#include "Character/Components/AttributeComponent.h"
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
	if (IsStagger)
	{
		Timer += DeltaTime;
	}
	if (IsDashing)
	{
		if (UKismetMathLibrary::Abs(GetVelocity().X) >= 3500.f)
		{
			DRAW_TEXT_ONSCREEN(GetVelocity().ToString());
			IsDashing = false;
		}
	}

	if (CurrentDashCount < DashLimit)
	{
		StartDashTimer(DeltaTime);
	}

	CheckAttack();
}

void ACHeroCharacter::SecondaryAttack()
{
}

void ACHeroCharacter::KeyboardKeyPressed()
{
	IsKeyboard = true;
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->SetShowMouseCursor(true);
	}
}

void ACHeroCharacter::GamepadKeyPressed()
{
	IsKeyboard = false;
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->SetShowMouseCursor(false);
	}
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
		PlayerController->SetShowMouseCursor(true);
	}

	FirstSkillSlotComponent->Equip(this, this);
	SecondSkillSlotComponent->Equip(this, this);
	UltimateSkillSlotComponent->Equip(this, this);

	CurrentDashCount = DashLimit;
	Tags.Add(FName("EngageableTarget"));
}

void ACHeroCharacter::ShakeCamera()
{
	GetLocalViewingPlayerController()->ClientStartCameraShake(CameraShake);
}

void ACHeroCharacter::ShakeCameraSkill()
{
	GetLocalViewingPlayerController()->ClientStartCameraShake(CameraShakeSkill);
}

void ACHeroCharacter::ResetStagger()
{
	IsStagger = false;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	DRAW_TEXT_ONSCREEN(FString::Printf(TEXT("%f"), Timer));
	Timer = 0.f;
}

void ACHeroCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (StaggerHandle.IsValid())
	{
		StaggerHandle.Invalidate();
	}
	GetWorldTimerManager().SetTimer(StaggerHandle, this, &ACHeroCharacter::ResetStagger,
	                                .8f, false);
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	IsStagger = true;

	// PlayHitSound(ImpactPoint);
	// SpawnHitParticles(ImpactPoint);
}

float ACHeroCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
	return DamageAmount;

}

void ACHeroCharacter::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			DeathPose = EDeathPose::EDP_DeathA;
			SectionName = FName("Death1");
			break;
		case 1:
			DeathPose = EDeathPose::EDP_DeathB;
			SectionName = FName("Death2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::RestartGameEvent,2.f, false);
}

float ACHeroCharacter::GetMovementAngle()
{
	float Angle = 0.f;
	float DotProduct = UKismetMathLibrary::DotProduct2D(MovementVector, FVector2D(0.f, 1.f));

	Angle = UKismetMathLibrary::DegAcos(DotProduct);
	if (MovementVector.X < 0)
	{
		Angle *= -1;
	}

	DRAW_TEXT_ONSCREEN(FString::Printf(TEXT("%f"), Angle));
	return Angle;
}

void ACHeroCharacter::StartDashTimer(float DeltaTime)
{
	if (DashTimer < DashRechargeRate)
	{
		DashTimer += DeltaTime;
		return;
	}
	DashTimer = 0.f;
	CurrentDashCount++;
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
		EnhancedInputComponent->BindAction(KeyboardAction, ETriggerEvent::Triggered, this,
		                                   &ACHeroCharacter::KeyboardKeyPressed);
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
	if (IsStagger)
	{
		return;
	}

	MovementVector = Value.Get<FVector2D>().GetRotated(-45.f).GetSafeNormal();
	// DRAW_TEXT_ONSCREEN(MovementVector.ToString());
	// float angle = GetMovementAngle();
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
	if (CurrentDashCount <= 0)
	{
		return;
	}
	SetActorRotation(FRotator(0.f, GetMovementAngle(), 0.f), ETeleportType::None);
	IsDashing = true;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACHeroCharacter::ResetDash,
	                                AnimationComponent->GetDashTime() - 0.05f, false);
	AnimationComponent->Dash(this, FVector(MovementVector.Y, MovementVector.X, 0.f).GetSafeNormal() * 300.f);
	EquippedWeapon->SetActorHiddenInGame(true);
	CurrentDashCount--;
}

void ACHeroCharacter::ResetDash()
{
	IsDashing = false;
	IsAttacking = false;
	EquippedWeapon->SetActorHiddenInGame(false);
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
	if (IsKeyboard)
	{
		float mouseX;
		float mouseY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
		FVector2D MousePos = FVector2D(mouseX, mouseY);

		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			FVector2D ScreenLocation;
			PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenLocation);
			FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(
				FVector(ScreenLocation.X, ScreenLocation.Y, 0.f), FVector(MousePos.X, MousePos.Y, 0.f));
			SetActorRotation(FRotator(0.f, LookAtRotator.Yaw + 135.f, 0.f));
		}
	}

	IsAttacking = true;
	if (AttackHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AttackHandle);
	}
	GetWorldTimerManager().SetTimer(AttackHandle, this, &ACHeroCharacter::ResetAttack,
	                                0.5f, false);
	//
	//
	//
	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//
	// if (AnimInstance && AttackMontage)
	// {
	// 	//AnimInstance->Sectio
	// 	// return;
	// 	if (!AnimInstance->Montage_IsPlaying(AttackMontage))
	// 	{
	// 		AttackState = 1;
	// 	}
	// 	else
	// 	{
	// 		// AnimInstance->StopSlotAnimation(0.35f);
	// 		AttackState++;
	//
	// 		if(AttackState > 3)
	// 		{
	// 			AttackState = 1;
	// 		}
	// 	}
	// 	AnimInstance->Montage_Play(AttackMontage);
	// 	FName SectionName = FName();
	//
	// 	switch (AttackState)
	// 	{
	// 	case 1:
	// 		SectionName = "SwordSlash1";
	// 		break;
	// 	case 2:
	// 		SectionName = "SwordSlash2";
	// 		break;
	// 	case 3:
	// 		SectionName = "SwordSlash3";
	// 		break;
	// 	default:
	// 		SectionName = "SwordSlash1";
	// 		break;
	// 	}
	// 	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	// }
}

void ACHeroCharacter::CameraMovementAnimation()
{
	AnimationComponent->CameraMovementAnimation(FollowCamera);
}

void ACHeroCharacter::CameraPostProcessAnimation()
{
	AnimationComponent->CameraPostProcessAnimation(FollowCamera);
}

void ACHeroCharacter::ResetAttack()
{
	IsAttacking = false;
	// DRAW_TEXT_ONSCREEN(TEXT("ResetAttack"));
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
	if (IsStagger) return;

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
	if (IsStagger) return;
	ShakeCameraSkill();
	FirstSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation(), this, this);
}

void ACHeroCharacter::CastSecondSkill()
{
	if (IsStagger) return;

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
	if (IsStagger) return;
	ShakeCameraSkill();
	SecondSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation(), this, this);
}

void ACHeroCharacter::CastUltimateSkill()
{
	if (IsStagger) return;

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
	if (IsStagger) return;
	ShakeCameraSkill();

	UltimateSkillSlotComponent->SpawnSkill(GetActorLocation(), GetActorRotation(), this, this);
}

void ACHeroCharacter::CheckAttack()
{
	if (IsStagger) return;

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

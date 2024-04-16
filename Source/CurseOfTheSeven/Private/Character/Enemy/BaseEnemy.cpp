// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BaseEnemy.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/AI/BaseAIComponent.h"
#include "Character/Components/AttributeComponent.h"
#include "Character/SkillComponent/SkillSlotComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Item/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/AnimationComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	// FloatingDamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingDamage"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	SkillSlotComponent = CreateDefaultSubobject<USkillSlotComponent>(TEXT("SkillSlot"));
	AnimationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthBarPercentage(1.f);
		HealthBarWidget->SetVisibility(false);
	}

	Tags.Add(FName("Enemy"));
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CombatTarget)
	{
		if (!InTargetRange(CombatTarget, CombatRadius))
		{
			CombatTarget = nullptr;
			SetHealthBarVisibility(false);		
		}
	}
}


// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead()) ShowHealthBar();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	
	StopAttackMontage();
}

bool ABaseEnemy::IsDead()
{
	return Attributes->IsAlive();
}

void ABaseEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}


void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthBarPercentage(Attributes->GetHealthPercent());
	}
	OnDamageTaken(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

AController* ABaseEnemy::GetCharacterController()
{
	return GetController();
}

void ABaseEnemy::Attack()
{
	Super::Attack();
}

void ABaseEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	Super::DirectionalHitReact(ImpactPoint);
}

void ABaseEnemy::SpawnAnimation()
{
	AnimationComponent->EnemySpawnAnimation(this);
}

bool ABaseEnemy::IsAttackPlaying()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)
	{
		if (AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			return true;
		}
	}
	return false;
}

bool ABaseEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	// DRAW_SPHERE_SingleFrame(GetActorLocation());
	// DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void ABaseEnemy::Die()
{
	// TODO: Override yapıp enemye gore calismak lazim
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			//DeathPose = EDeathPose::EDP_DeathA;
			SectionName = FName("Death1");
			break;
		case 1:
			//DeathPose = EDeathPose::EDP_DeathB;
			SectionName = FName("Death2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	SetDead();
}

void ABaseEnemy::SetHealthBarVisibility(const bool IsVisible) const
{
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(IsVisible);
	}
}

void ABaseEnemy::SetDead()
{
	SetHealthBarVisibility(false);	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(2.f);
}




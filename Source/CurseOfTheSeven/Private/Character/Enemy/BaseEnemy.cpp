// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BaseEnemy.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/AI/BaseAIComponent.h"
#include "Character/Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"

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

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	BaseAI = CreateDefaultSubobject<UBaseAIComponent>(TEXT("AIComponent"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
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

void ABaseEnemy::PlayHitReactMontage(const FName& SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	if (HitParticlesSystem && GetWorld())
	{
		HitParticleInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), HitParticlesSystem, ImpactPoint);

		HitParticleInstance->Activate();
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
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

AController* ABaseEnemy::GetCharacterController()
{
	return GetController();
}

bool ABaseEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
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
	SetHealthBarVisibility(false);	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(2.0f);
}

void ABaseEnemy::DirectionalHitReact(const FVector& ImpactPoint) const
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLower(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLower - GetActorLocation()).GetSafeNormal();

	const double AngleCosine = FVector::DotProduct(Forward, ToHit);
	double Angle = FMath::Acos(AngleCosine);
	Angle = FMath::RadiansToDegrees(Angle);

	//If cross product points down, angle should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Angle *= -1.f;
	}

	FName Section("FrontHit");

	if (Angle >= -45.f && Angle < -135.f)
	{
		Section = FName("LeftHit");
	}
	else if (Angle >= 45.f && Angle < 135.f)
	{
		Section = FName("RightHit");
	}

	PlayHitReactMontage(Section);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green,
		                                 FString::Printf(TEXT("%s: %f"), *Section.ToString(), Angle));
	}
}

void ABaseEnemy::SetHealthBarVisibility(const bool IsVisible) const
{
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(IsVisible);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/LichEnemy.h"

#include "Character/AI/LichAIComponent.h"
#include "Character/Components/AttributeComponent.h"
#include "Character/SkillComponent/SkillSlotComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "Kismet/KismetMathLibrary.h"


ALichEnemy::ALichEnemy()
{
	SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(GetRootComponent());
	SkillCastPoint = CreateDefaultSubobject<USceneComponent>("SkillCastPoint");
	SkillCastPoint->SetupAttachment(GetRootComponent());

	BaseAI = CreateDefaultSubobject<ULichAIComponent>(TEXT("AIComponent"));

}

void ALichEnemy::BeginPlay()
{
	Super::BeginPlay();
	BaseAI->InitializeAI();
	SpawnSkeleton();
}

void ALichEnemy::Attack()
{
	Super::Attack();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BaseAI->GetPlayerLocation()));
	PlayAttackMontage();
}

void ALichEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SkillTimer += DeltaSeconds;
	SpawnTimer += DeltaSeconds;
}

void ALichEnemy::SpawnSkeleton()
{
	UWorld* World = GetWorld();

	if(!World)
	{
		DRAW_TEXT_ONSCREEN(TEXT("No World"));
		return;
	}
	ASkeletonEnemy* SpawnedSkeleton = World->SpawnActor<ASkeletonEnemy>(SkeletonPrefab, SpawnPoint->GetComponentLocation() + FVector(0.f, 0.f, 200.f), GetActorRotation());
	SpawnedSkeleton->SpawnAnimation();
}

void ALichEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	Super::DirectionalHitReact(ImpactPoint);
}

void ALichEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
	SetDead();
}

void ALichEnemy::SpawnSkill()
{
	SkillSlotComponent->SpawnSkill(SkillCastPoint->GetComponentLocation(), GetActorRotation(), this, this);
}

void ALichEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	DRAW_TEXT_ONSCREEN(TEXT("Attack"));

	if(Attributes->GetHealthPercent() <= .5f)
	{
		DRAW_TEXT_ONSCREEN(TEXT("Normal Attack"));
		
		BaseAI->AttackRadius = 100.f;
		if (AnimInstance && AttackMontage)
		{
			if (AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				return;
			}
			AnimInstance->Montage_Play(AttackMontage);
			const int32 Selection = FMath::RandRange(0, 1);
			FName SectionName = FName();
			switch (Selection)
			{
			case 0:
				SectionName = FName("Attack1");
				break;
			case 1:
				SectionName = FName("Attack2");
				break;
			default:
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		}
		
		return;
	}

	BaseAI->AttackRadius = 10000.f;

	if(CheckSpawn())
	{
		DRAW_TEXT_ONSCREEN(TEXT("Spawn Attack"));

		if (AnimInstance && SpawnMontage)
		{
			if (AnimInstance->Montage_IsPlaying(SpawnMontage))
			{
				return;
			}
			AnimInstance->Montage_Play(SpawnMontage);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ALichEnemy::SpawnSkeleton,
											1.f, false);
		}

		return;
	}

	if(CheckSkills())
	{
		DRAW_TEXT_ONSCREEN(TEXT("Skill Attack"));

		if (AnimInstance && SkillSlotComponent->GetSkillMontage())
		{
			if (AnimInstance->Montage_IsPlaying(SkillSlotComponent->GetSkillMontage()))
			{
				return;
			}
			AnimInstance->Montage_Play(SkillSlotComponent->GetSkillMontage());

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ALichEnemy::SpawnSkill,
											SkillSlotComponent->GetDelay(), false);
		}
	}
	
}

bool ALichEnemy::CheckSkills()
{
	if(SkillTimer >= 2.f)
	{
		SkillTimer = 0.f;
		return true;
	}

	return false;
}

bool ALichEnemy::CheckSpawn()
{
	if(SpawnTimer >= 30.f)
	{
		SpawnSkeleton();
		SpawnTimer = 0.f;
		SkillTimer = 0.f;
		return true;
	}

	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/ArcherSkeletonEnemy.h"

#include "Character/AI/BaseAIComponent.h"
#include "Character/SkillComponent/SkillSlotComponent.h"
#include "Kismet/KismetMathLibrary.h"

AArcherSkeletonEnemy::AArcherSkeletonEnemy()
{
	BaseAI = CreateDefaultSubobject<UBaseAIComponent>(TEXT("AIComponent"));
	ArrowCastPoint = CreateDefaultSubobject<USceneComponent>("SkillCastPoint");
	ArrowCastPoint->SetupAttachment(GetRootComponent());

}

void AArcherSkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();
	BaseAI->InitializeAI();
}

void AArcherSkeletonEnemy::Attack()
{
	Super::Attack();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BaseAI->GetPlayerLocation()));
	PlayAttackMontage();
}

void AArcherSkeletonEnemy::SpawnSkill()
{
	SkillSlotComponent->SpawnSkill(ArrowCastPoint->GetComponentLocation(), GetActorRotation(), this, this);
}

void AArcherSkeletonEnemy::Die()
{
	Super::Die();
}

void AArcherSkeletonEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		if (AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			return;
		}
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("Attack"), AttackMontage);
		OnAttack();

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AArcherSkeletonEnemy::SpawnSkill,
											.40f, false);
	}
}

void AArcherSkeletonEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FName Section("Hit");
	PlayHitReactMontage(Section);
	BaseAI->SetStagger();

}
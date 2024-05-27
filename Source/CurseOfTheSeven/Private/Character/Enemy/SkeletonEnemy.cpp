// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/SkeletonEnemy.h"

#include "Character/AI/BaseAIComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"

ASkeletonEnemy::ASkeletonEnemy()
{
	BaseAI = CreateDefaultSubobject<UBaseAIComponent>(TEXT("AIComponent"));
}

void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();
	BaseAI->InitializeAI();
}

void ASkeletonEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

void ASkeletonEnemy::Die()
{
	Super::Die();
}

void ASkeletonEnemy::PlayAttackMontage()
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
}

void ASkeletonEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const int32 Selection = FMath::RandRange(0, 2);
	FName SectionName = FName();
	switch (Selection)
	{
	case 0:
		SectionName = FName("Hit1");
		break;
	case 1:
		SectionName = FName("Hit2");
		break;
	case 2:
		SectionName = FName("Hit3");
		break;
	default:
		break;
	}
	PlayHitReactMontage(SectionName);
	BaseAI->SetStagger();

}


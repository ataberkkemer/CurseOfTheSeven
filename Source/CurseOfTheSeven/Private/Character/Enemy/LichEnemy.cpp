// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/LichEnemy.h"

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

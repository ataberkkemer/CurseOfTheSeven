// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BaseEnemy.h"
#include "SkeletonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API ASkeletonEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ASkeletonEnemy();
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Die() override;
	
	virtual void PlayAttackMontage() override;
	

protected:

	virtual void DirectionalHitReact(const FVector& ImpactPoint) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BaseEnemy.h"
#include "LichEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API ALichEnemy : public ABaseEnemy
{
	GENERATED_BODY()

protected:
	virtual void DirectionalHitReact(const FVector& ImpactPoint) override;
	virtual void Die() override;
};

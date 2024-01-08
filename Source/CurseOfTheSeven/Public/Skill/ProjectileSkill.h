// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/BaseSkill.h"
#include "ProjectileSkill.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API AProjectileSkill : public ABaseSkill
{
	GENERATED_BODY()

public:
	AProjectileSkill();

	virtual void BeginPlay() override;
	
protected:
	
private:
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	float MaxSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	float Gravity;

};

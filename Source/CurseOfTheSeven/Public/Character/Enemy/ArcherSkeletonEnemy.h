// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BaseEnemy.h"
#include "ArcherSkeletonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API AArcherSkeletonEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public:
	AArcherSkeletonEnemy();
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Die() override;
	
	virtual void PlayAttackMontage() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnAttack();

	UPROPERTY(EditAnywhere)
	float ArrowCastDelay;
protected:

	virtual void DirectionalHitReact(const FVector& ImpactPoint) override;
	virtual void SpawnSkill() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ArrowCastPoint;
};

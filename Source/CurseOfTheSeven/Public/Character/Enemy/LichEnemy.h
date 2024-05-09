// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkeletonEnemy.h"
#include "Character/Enemy/BaseEnemy.h"
#include "LichEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API ALichEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	ALichEnemy();
	
public:
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Tick(float DeltaSeconds) override;
	void SpawnSkeleton();
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* SpawnMontage;
	
protected:
	virtual void DirectionalHitReact(const FVector& ImpactPoint) override;
	virtual void Die() override;
	void SpawnSkill() override;
	virtual void PlayAttackMontage() override;
	bool CheckSkills();
	bool CheckSpawn();


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASkeletonEnemy> SkeletonPrefab;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SkillCastPoint;

	float SkillTimer;
	float SpawnTimer;
	

};

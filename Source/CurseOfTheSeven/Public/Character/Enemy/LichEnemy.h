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
	void SpawnSkeleton();
	
protected:
	virtual void DirectionalHitReact(const FVector& ImpactPoint) override;
	virtual void Die() override;
	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASkeletonEnemy> SkeletonPrefab;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnPoint;
};

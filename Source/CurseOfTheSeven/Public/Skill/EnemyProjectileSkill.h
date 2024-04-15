// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/BaseSkill.h"
#include "EnemyProjectileSkill.generated.h"

class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API AEnemyProjectileSkill : public ABaseSkill
{
	GENERATED_BODY()
public:
	AEnemyProjectileSkill();
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;
};

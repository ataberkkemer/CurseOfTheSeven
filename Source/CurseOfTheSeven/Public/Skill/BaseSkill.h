// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"
#include "BaseSkill.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;
class UAnimMontage;
class USkillAttribiuteComponent;

UCLASS()
class CURSEOFTHESEVEN_API ABaseSkill : public AActor
{
	GENERATED_BODY()

public:
	ABaseSkill();

	virtual void BeginPlay() override;

	UFUNCTION()
	void SetAttributes();
	
	UPROPERTY(EditDefaultsOnly)
	float Delay;
	
	UPROPERTY(VisibleDefaultsOnly)
	UNiagaraComponent* SkillEffect;

	UPROPERTY(EditAnywhere, Category = "Active Effects")
	USoundBase* ActiveSound;

	UPROPERTY(EditAnywhere, Category = "Hit Effects")
	UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Hit Effects")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* SkillAnimation;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Hit Effects")
	UNiagaraComponent* HitParticleInstance;

	UPROPERTY(EditAnywhere)
	float LifeSpan;

	UPROPERTY(VisibleAnywhere)
	USkillAttribiuteComponent* Attributes;

	int SkillIndex;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraActor.h"
#include "BaseSkill.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;
class USkillAttribiuteComponent;

UCLASS()
class CURSEOFTHESEVEN_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseSkill();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	UNiagaraComponent* SkillEffect;
	
	UPROPERTY(EditAnywhere, Category = "Active Effects")
	USoundBase* ActiveSound;
	
	UPROPERTY(EditAnywhere, Category = "Hit Effects")
	UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Hit Effects")
	USoundBase* HitSound;
	

protected:
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Hit Effects")
	UNiagaraComponent* HitParticleInstance;

	UPROPERTY(EditAnywhere)
	float LifeSpan;
	
	UPROPERTY(VisibleAnywhere)
	USkillAttribiuteComponent* Attributes;
};

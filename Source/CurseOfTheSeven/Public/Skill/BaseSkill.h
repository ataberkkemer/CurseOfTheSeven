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
	void Equip(AActor* NewOwner, APawn* NewInstigator);

	void SetAttributes(float RawDamage, float ElementalDamage, float DamageTickInterval, float StaggerDamage);
	void SphereTrace(FHitResult& SphereHit);
	void DisableActor(bool toHide);
	void ApplyDamage(FHitResult SphereHit);
	void ExecuteMultipleHitTry(FHitResult HitResult);
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditDefaultsOnly)
	float Delay;
	
	UPROPERTY(VisibleDefaultsOnly)
	UNiagaraComponent* SkillEffect;
	
	UPROPERTY(EditAnywhere)
	bool IsMultipleHitSkill;

	UPROPERTY(EditAnywhere, Category = "Active Effects")
	USoundBase* ActiveSound;

	UPROPERTY(EditAnywhere, Category = "Hit Effects")
	UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Hit Effects")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* SkillAnimation;

	UPROPERTY(VisibleAnywhere)
	USkillAttribiuteComponent* Attributes;
	
	UPROPERTY(VisibleAnywhere)
    	USceneComponent* SphereTraceStart;
    	
    	UPROPERTY(VisibleAnywhere)
    	USceneComponent* SphereTraceEnd;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision;

	void ExecuteMultipleHit(FHitResult& SphereHit);
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereOverlapEnd();

	bool ActorIsSameType(AActor* OtherActor);
	void ExecuteGetHit(FHitResult& SphereHit);
	void RespawnPlayer(APlayerController* PlayerController);
	void RespawnPlayerWithDelay(FHitResult Player);

private:
	UPROPERTY(VisibleAnywhere, Category = "Hit Effects")
	UNiagaraComponent* HitParticleInstance;

	UPROPERTY(EditAnywhere)
	float LifeSpan;
	
	

	int SkillIndex;
};

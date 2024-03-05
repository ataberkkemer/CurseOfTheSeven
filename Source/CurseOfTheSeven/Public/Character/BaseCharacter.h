// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UNiagaraComponent;
class AWeapon;
class UAttributeComponent;
class UAnimMontage;
class UNiagaraSystem;

UCLASS()
class CURSEOFTHESEVEN_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void SetWeaponVFXEnabled(bool isEnabled);


	virtual void Attack();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	bool IsAlive();

	virtual void Die();
	virtual void DirectionalHitReact(const FVector& ImpactPoint);
	void StopAttackMontage();

protected:
	virtual void BeginPlay() override;
	
	virtual void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);
	
	virtual bool CanAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual void SpawnDefaultWeapon();

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> WeaponClass;
	
	
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	// UPROPERTY(BlueprintReadOnly)
	// TEnumAsByte<EDeathPose> DeathPose;
	
	UNiagaraComponent* HitParticleInstance;

	/**
	* Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	/*
	* Components
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UNiagaraSystem* HitParticle;

	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);

	void DisableCapsule();
};

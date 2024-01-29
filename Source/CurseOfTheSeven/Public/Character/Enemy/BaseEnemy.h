// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseEnemy.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UBaseAIComponent;

UCLASS()
class CURSEOFTHESEVEN_API ABaseEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	bool InTargetRange(AActor* Target, double Radius);
	AController* GetCharacterController();
	
protected:
	virtual void BeginPlay() override;
	void PlayHitReactMontage(const FName& SectionName) const;
	
	UNiagaraComponent* HitParticleInstance;
	
	virtual void Die();
	virtual void DirectionalHitReact(const FVector& ImpactPoint) const;

	void SetHealthBarVisibility(const bool IsVisible) const;
	void SetDead();
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UNiagaraSystem* HitParticlesSystem;
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	UBaseAIComponent* BaseAI;
	
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	
};

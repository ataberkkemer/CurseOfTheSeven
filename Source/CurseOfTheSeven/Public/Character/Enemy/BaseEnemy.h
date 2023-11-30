// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseEnemy.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;

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
	
protected:
	virtual void BeginPlay() override;
	void PlayHitReactMontage(const FName& SectionName) const;
	void Die();
	UNiagaraComponent* HitParticleInstance;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UNiagaraSystem* HitParticlesSystem;
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	
	void DirectionalHitReact(const FVector& ImpactPoint);
};

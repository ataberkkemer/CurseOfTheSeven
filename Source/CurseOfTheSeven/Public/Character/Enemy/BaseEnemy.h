// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseEnemy.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UAnimMontage;

UCLASS()
class CURSEOFTHESEVEN_API ABaseEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UNiagaraSystem* HitParticlesSystem;
	
	
	void DirectionalHitReact(const FVector& ImpactPoint);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PlayHitReactMontage(const FName& SectionName) const;
	UNiagaraComponent* HitParticleInstance;
};

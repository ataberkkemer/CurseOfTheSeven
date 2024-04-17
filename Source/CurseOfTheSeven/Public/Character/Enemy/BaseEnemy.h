// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterTypes.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class USkillSlotComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UBaseAIComponent;
class UAnimationComponent;

UCLASS()
class CURSEOFTHESEVEN_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	bool IsDead();
	void ShowHealthBar();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	bool InTargetRange(AActor* Target, double Radius);
	AController* GetCharacterController();

	virtual void Attack() override;
	virtual void DirectionalHitReact(const FVector& ImpactPoint) override;
	virtual void SpawnAnimation();
	bool IsAttackPlaying();
	
protected:
	virtual void BeginPlay() override;
	
	
	virtual void Die() override;

	void SetHealthBarVisibility(const bool IsVisible) const;
	void SetDead();

	UPROPERTY(VisibleAnywhere)
	UBaseAIComponent* BaseAI;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnDamageTaken(int Damage);

	UPROPERTY(VisibleAnywhere)
	USkillSlotComponent* SkillSlotComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthBarComponent* HealthBarWidget;
private:
	
	
	UPROPERTY(EditAnywhere)
	UUserWidget* DamageWidget;
	
	UPROPERTY(EditAnywhere)
	UAnimationComponent* AnimationComponent;
	
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	
	
};

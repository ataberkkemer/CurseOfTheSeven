// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BaseEnemy.h"
#include "Components/ActorComponent.h"
#include "BaseAIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTHESEVEN_API UBaseAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseAIComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetStagger();
	void ResetStagger();
	void InitializeAI();

protected:
	virtual void BeginPlay() override;
	virtual void PlayerInitiate();
	virtual void CheckPlayer();

	UPROPERTY()
	class AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "Navigation")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "Navigation")
	TArray<AActor*> AllPatrolTargets;

	UPROPERTY(EditAnywhere, Category = "Navigation")
	double AttackRadius;
	
	UPROPERTY()
	ACharacter* Player;

	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	ABaseEnemy* BaseEnemy = nullptr;
private:	
	UPROPERTY(VisibleAnywhere, Category = "Variables")

	FTimerHandle PatrolTimer;
	FTimerHandle StaggerTimer;

	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;
	

	EEnemyState EnemyState = EEnemyState::EES_Chasing;
	
	void CheckPatrolTarget();
};

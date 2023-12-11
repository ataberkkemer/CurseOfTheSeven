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

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "Navigation")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "Navigation")
	TArray<AActor*> AllPatrolTargets;

	UPROPERTY(EditAnywhere, Category = "Navigation")
	double PatrolRadius;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Variables")
	ABaseEnemy* BaseEnemy;
	
	
		
};

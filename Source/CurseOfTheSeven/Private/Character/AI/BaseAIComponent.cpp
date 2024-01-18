// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BaseAIComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "Character/Enemy/BaseEnemy.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
UBaseAIComponent::UBaseAIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBaseAIComponent::BeginPlay()
{
	Super::BeginPlay();

	BaseEnemy = Cast<ABaseEnemy>(GetOwner());
	EnemyController = Cast<AAIController>(BaseEnemy->GetCharacterController());
	
	MoveToTarget(CurrentPatrolTarget);
}


void UBaseAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//TODO: Her tickde yapmanın anlamı yok
	//CheckPatrolTarget();
}

void UBaseAIComponent::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* UBaseAIComponent::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : AllPatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void UBaseAIComponent::CheckPatrolTarget()
{
	if (BaseEnemy->InTargetRange(CurrentPatrolTarget, PatrolRadius))
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		BaseEnemy->GetWorldTimerManager().SetTimer(PatrolTimer, this, &UBaseAIComponent::PatrolTimerFinished, WaitTime);
	}
}


void UBaseAIComponent::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BaseAIComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "Character/Enemy/BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
UBaseAIComponent::UBaseAIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBaseAIComponent::PlayerInitiate()
{
	if (Player)
	{
		MoveToTarget(Player);
	}
}

void UBaseAIComponent::InitializeAI()
{
	BaseEnemy = Cast<ABaseEnemy>(GetOwner());
	EnemyController = Cast<AAIController>(BaseEnemy->GetCharacterController());
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	PlayerInitiate();
}

void UBaseAIComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBaseAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	//TODO: Her tickde yapmanın anlamı yok

	CheckPlayer();

	if ((BaseEnemy && BaseEnemy->IsAttackPlaying()) || EnemyState == EEnemyState::EES_Stagger)
	{
		BaseEnemy->GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else
	{
		BaseEnemy->GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
}

void UBaseAIComponent::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyController->MoveTo(MoveRequest);
	BaseEnemy->GetCharacterMovement()->MaxWalkSpeed = 400.f;
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
	if (BaseEnemy->InTargetRange(CurrentPatrolTarget, AttackRadius))
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		BaseEnemy->GetWorldTimerManager().SetTimer(PatrolTimer, this, &UBaseAIComponent::PatrolTimerFinished, WaitTime);
	}
}

void UBaseAIComponent::CheckPlayer()
{
	if (EnemyState == EEnemyState::EES_Stagger)
	{
		return;
	}

	if (!BaseEnemy->InTargetRange(Player, AttackRadius))
	{
		MoveToTarget(Player);
		EnemyState = EEnemyState::EES_Chasing;
		BaseEnemy->GetWorldTimerManager().SetTimer(PatrolTimer, this, &UBaseAIComponent::PatrolTimerFinished, 1.f);
	}
	else
	{
		EnemyState = EEnemyState::EES_Attacking;
		BaseEnemy->Attack();
	}
}

void UBaseAIComponent::PatrolTimerFinished()
{
	MoveToTarget(Player);
}

void UBaseAIComponent::SetStagger()
{
	EnemyState = EEnemyState::EES_Stagger;

	if (StaggerTimer.IsValid())
	{
		BaseEnemy->GetWorldTimerManager().ClearTimer(StaggerTimer);
	}
	
	BaseEnemy->GetWorldTimerManager().SetTimer(StaggerTimer, this, &UBaseAIComponent::ResetStagger, 2.f);
}

void UBaseAIComponent::ResetStagger()
{
	EnemyState = EEnemyState::EES_Chasing;
}

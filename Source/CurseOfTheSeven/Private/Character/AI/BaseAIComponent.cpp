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
	
	if (EnemyController && CurrentPatrolTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(CurrentPatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}
	}
}


// Called every frame
void UBaseAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	if (CurrentPatrolTarget && EnemyController)
	{
		if (BaseEnemy->InTargetRange(CurrentPatrolTarget, PatrolRadius))
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
				AActor* Target = ValidTargets[TargetSelection];
				CurrentPatrolTarget = Target;

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(CurrentPatrolTarget);
				MoveRequest.SetAcceptanceRadius(15.f);
				EnemyController->MoveTo(MoveRequest);
			}
		}
	}
}

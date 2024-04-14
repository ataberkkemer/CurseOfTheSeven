// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/LichEnemy.h"

#include "Character/AI/LichAIComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"


ALichEnemy::ALichEnemy()
{
	SpawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(GetRootComponent());

	BaseAI = CreateDefaultSubobject<ULichAIComponent>(TEXT("AIComponent"));

}

void ALichEnemy::BeginPlay()
{
	Super::BeginPlay();
	BaseAI->InitializeAI();
	SpawnSkeleton();
}

void ALichEnemy::Attack()
{
	Super::Attack();
}

void ALichEnemy::SpawnSkeleton()
{
	UWorld* World = GetWorld();

	if(!World)
	{
		DRAW_TEXT_ONSCREEN(TEXT("No World"));
		return;
	}
	ASkeletonEnemy* SpawnedSkeleton = World->SpawnActor<ASkeletonEnemy>(SkeletonPrefab, SpawnPoint->GetComponentLocation() + FVector(0.f, 0.f, 200.f), GetActorRotation());
	SpawnedSkeleton->SpawnAnimation();
}

void ALichEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	Super::DirectionalHitReact(ImpactPoint);
}

void ALichEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
	SetDead();
}

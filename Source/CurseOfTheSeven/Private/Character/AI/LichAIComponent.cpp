// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/LichAIComponent.h"

void ULichAIComponent::BeginPlay()
{
	Super::BeginPlay();
	LichEnemy = Cast<ALichEnemy>(BaseEnemy);
}

void ULichAIComponent::PlayerInitiate()
{
	// LichEnemy->SpawnSkeleton();
}

void ULichAIComponent::CheckPlayer()
{
	
}

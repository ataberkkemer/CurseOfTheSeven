// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AI/BaseAIComponent.h"
#include "Character/Enemy/LichEnemy.h"
#include "LichAIComponent.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API ULichAIComponent : public UBaseAIComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
protected:
	virtual void PlayerInitiate() override;
	virtual void CheckPlayer() override;
	
private:
	ALichEnemy* LichEnemy = nullptr;
};

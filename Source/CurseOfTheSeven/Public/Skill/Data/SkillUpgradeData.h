// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillUpgrade.h"
#include "Character/Upgrades/Skill/UpgradeTypes.h"
#include "Engine/DataAsset.h"
#include "SkillUpgradeData.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTHESEVEN_API USkillUpgradeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SkillUpgrades")
	TArray<FSkillUpgrade> SkillUpgradeData;

};



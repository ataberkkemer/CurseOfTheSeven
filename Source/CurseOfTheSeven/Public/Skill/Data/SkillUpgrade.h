#pragma once
#include "Skill/BaseSkill.h"
#include "SkillUpgrade.generated.h"
enum class UpgradeTypes : uint8;

USTRUCT(BlueprintType)
struct FSkillUpgrade
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UpgradeTypes SkillUpgradeType;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* UpgradedSkill;
	
	UPROPERTY(EditAnywhere)
	float UpgradeValue;
};

USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_BODY()
	
public:

	float RawDamage;
	float ElementalDamage;
	float ElementalTickInterval;
	float StaggerDamage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillUpgradeComponent.generated.h"


class USkillUpgradeData;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTHESEVEN_API USkillUpgradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillUpgradeComponent();

	UFUNCTION(CallInEditor)
	void ApplyDamageUpgrade(class ABaseSkill* Skill, float value);

	UFUNCTION(CallInEditor)
	void ApplySizeUpgrade(class ABaseSkill* Skill, float value);

	UFUNCTION(CallInEditor)
	void ApplyDoubleCasting(class ABaseSkill* Skill);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<USkillUpgradeData*> AllUpgradeTypes;
	
	UPROPERTY(VisibleAnywhere)
	TArray<USkillUpgradeData*> OwnedUpgradeTypes;
};

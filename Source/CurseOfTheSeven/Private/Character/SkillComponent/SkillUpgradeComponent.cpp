// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SkillComponent/SkillUpgradeComponent.h"

#include "Skill/BaseSkill.h"

// Sets default values for this component's properties
USkillUpgradeComponent::USkillUpgradeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USkillUpgradeComponent::ApplyDamageUpgrade(ABaseSkill* Skill, float value)
{
}

void USkillUpgradeComponent::ApplySizeUpgrade(ABaseSkill* Skill, float value)
{
}

void USkillUpgradeComponent::ApplyDoubleCasting(ABaseSkill* Skill)
{
}


// Called when the game starts
void USkillUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USkillUpgradeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


#include "Character/SkillComponent/SkillSlotComponent.h"

#include "CurseOfTheSeven/DebugMacros.h"
#include "Skill/BaseSkill.h"

USkillSlotComponent::USkillSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USkillSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
}


void USkillSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillSlotComponent::SpawnSkill(FVector Position, FRotator Rotation)
{
	
	if (OwnerActor->GetWorld() && SlotSkill)
	{
		ABaseSkill* SpawnedSkill = OwnerActor->GetWorld()->SpawnActor<ABaseSkill>(SlotSkill, Position + FVector(100.F,100.F,0.F), Rotation);
		SkillInstances.AddUnique(SpawnedSkill);
	}
}


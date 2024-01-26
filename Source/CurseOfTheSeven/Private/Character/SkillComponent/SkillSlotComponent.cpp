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


void USkillSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillSlotComponent::SpawnSkill(FVector Position, FRotator Rotation)
{
	if (OwnerActor->GetWorld() && SlotSkill)
	{
		ABaseSkill* SpawnedSkill = OwnerActor->GetWorld()->SpawnActor<ABaseSkill>(SlotSkill, Position, Rotation);
		SkillInstances.AddUnique(SpawnedSkill);
	}
}

UAnimMontage* USkillSlotComponent::GetSkillMontage() const
{
	if (SlotSkill)
	{
		return SlotSkill.GetDefaultObject()->SkillAnimation;
	}

	return nullptr;
}

float USkillSlotComponent::GetDelay() const
{
	if (SlotSkill)
	{
		return SlotSkill.GetDefaultObject()->Delay;
	}
	return 0.f;
}

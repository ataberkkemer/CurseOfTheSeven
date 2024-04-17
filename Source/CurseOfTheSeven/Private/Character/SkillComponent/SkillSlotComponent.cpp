#include "Character/SkillComponent/SkillSlotComponent.h"

#include "NiagaraComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "Skill/BaseSkill.h"
#include "Skill/Components/SkillAttribiuteComponent.h"

USkillSlotComponent::USkillSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USkillSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	SetSkill(nullptr);
}


void USkillSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillSlotComponent::SpawnSkill(FVector Position, FRotator Rotation, AActor* NewOwner, APawn* NewInstigator)
{
	if (OwnerActor->GetWorld() && SlotSkill)
	{
		ABaseSkill* SpawnedSkill = OwnerActor->GetWorld()->SpawnActor<ABaseSkill>(SlotSkill, Position, Rotation);
		SpawnedSkill->DisableActor(true);
		SpawnedSkill->Equip(NewOwner,NewInstigator);
		SpawnedSkill->DisableActor(false);

		if(UpgradedParticle)
		{
			SpawnedSkill->SkillEffect->SetAsset(UpgradedParticle);
			SpawnedSkill->SetAttributes(AttributeData.RawDamage, AttributeData.ElementalDamage, AttributeData.ElementalTickInterval, AttributeData.StaggerDamage);
		}
		SkillInstances.AddUnique(SpawnedSkill);
	}
}

void USkillSlotComponent::SetUpgrade(UNiagaraSystem* Upgraded, float RawDamageMultiplier,
	float ElementalDamageMultiplier, float StaggerDamageMultiplier)
{
	UpgradedParticle = Upgraded;
}

void USkillSlotComponent::SetSkill(ABaseSkill* BaseSkill)
{
	if(SlotSkill)
	{
		AttributeData.RawDamage = SlotSkill.GetDefaultObject()->Attributes->GetRawDamage();
		AttributeData.ElementalDamage = SlotSkill.GetDefaultObject()->Attributes->GetElementalDamage();
		AttributeData.StaggerDamage = SlotSkill.GetDefaultObject()->Attributes->GetStaggerDamage();

	}
}

void USkillSlotComponent::Equip(AActor* NewOwner, APawn* NewIns)
{
	if(SlotSkill)
	{
		SlotSkill.GetDefaultObject()->SetOwner(NewOwner);
		SlotSkill.GetDefaultObject()->SetInstigator(NewIns);

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

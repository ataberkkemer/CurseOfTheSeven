// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/Data/SkillUpgrade.h"
#include "SkillSlotComponent.generated.h"
class ABaseSkill;
class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTHESEVEN_API USkillSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillSlotComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnSkill(FVector Position, FRotator Rotation, AActor* NewOwner, APawn* NewInstigator);
	void SetUpgrade(UNiagaraSystem* UpgradedParticle, float RawDamageMultiplier, float ElementalDamageMultiplier, float StaggerDamageMultiplier);
	void SetSkill(ABaseSkill* BaseSkill);
	void Equip(AActor* NewOwner, APawn* NewIns);

	FORCEINLINE UAnimMontage* GetSkillMontage() const;
	FORCEINLINE float GetDelay() const;
	FORCEINLINE TSubclassOf<ABaseSkill> GetSkill() const;
	FORCEINLINE FAttributeData GetAttributeData() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	AActor* OwnerActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseSkill> SlotSkill;
	
	UPROPERTY()
	UNiagaraSystem* UpgradedParticle; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TArray<ABaseSkill*> SkillInstances;
	
	FAttributeData AttributeData;
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillAttribiuteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTHESEVEN_API USkillAttribiuteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillAttribiuteComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Getters
	FORCEINLINE float GetRawDamage() const { return Damage; }
	FORCEINLINE float GetElementalDamage() const { return ElementalDamage; }
	FORCEINLINE float GetStaggerDamage() const { return StaggerDamage; }
	
protected:
	virtual void BeginPlay() override;
private:

	UPROPERTY(EditAnywhere)
	float Damage;
	
	UPROPERTY(EditAnywhere)
	float StaggerDamage;

	UPROPERTY(EditAnywhere)
	float ElementalDamage;
		
};

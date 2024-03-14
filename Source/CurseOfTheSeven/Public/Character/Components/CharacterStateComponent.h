// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterTypes.h"
#include "Components/ActorComponent.h"
#include "CharacterStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTHESEVEN_API UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterStateComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//Getter
	UFUNCTION()
	FORCEINLINE ECharacterMovementState GetCharacterMovementState() const { return CharacterMovementState; }
	
	//Setter
	UFUNCTION()
	FORCEINLINE void SetCharacterStateToRunning() { CharacterMovementState = ECharacterMovementState::ECMS_Running; }

	UFUNCTION()
	FORCEINLINE void SetCharacterStateToDashing(){ CharacterMovementState = ECharacterMovementState::ECMS_Dashing; }
	
	UFUNCTION()
	FORCEINLINE void SetCharacterStateToAttacking(){ CharacterMovementState = ECharacterMovementState::ECMS_Attacking; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ECharacterMovementState CharacterMovementState;
private:
		
};

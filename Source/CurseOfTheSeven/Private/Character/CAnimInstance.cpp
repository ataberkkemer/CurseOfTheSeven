// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAnimInstance.h"

#include "Character/CHeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	HeroCharacter = Cast<ACHeroCharacter>(TryGetPawnOwner());

	if(HeroCharacter)
	{
		HeroCharacterMovementComponent = HeroCharacter->GetCharacterMovement();
	}
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(HeroCharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(HeroCharacterMovementComponent->Velocity);
		CharacterState = HeroCharacter->GetCharacterState();
	}
}

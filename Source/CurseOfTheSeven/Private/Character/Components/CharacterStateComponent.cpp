#include "Character/Components/CharacterStateComponent.h"

UCharacterStateComponent::UCharacterStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


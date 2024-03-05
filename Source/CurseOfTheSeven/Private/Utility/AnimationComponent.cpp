// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AnimationComponent.h"

#include "Components/CapsuleComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/Character.h"

UAnimationComponent::UAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurveFloatTimeline.TickTimeline(DeltaTime);
}

void UAnimationComponent::TimeLineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartPosition, EndPosition, Value);
	FHitResult* HitResult = nullptr;
	AnimatedActor->SetActorLocation(NewLocation, true, HitResult, ETeleportType::None);
}

void UAnimationComponent::Dash(ACharacter* Actor, FVector Offset)
{
	if(CurveFloat && !CurveFloatTimeline.IsPlaying())
	{
		AnimatedActor = Actor;
		StartPosition = EndPosition = AnimatedActor->GetActorLocation();
		EndPosition += Offset;
		
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveFloatTimeline.AddInterpFloat(CurveFloat, TimeLineProgress);
		CurveFloatTimeline.SetLooping(false);
		CurveFloatTimeline.PlayFromStart();
		
		// FTimerHandle UnusedHandle;
		// AnimatedActor->GetWorldTimerManager().SetTimer(UnusedHandle,this, &UAnimationComponent::DashCallback, 1.f);

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Time %f"),));
	}
}

float UAnimationComponent::GetDashTime()
{
	return CurveFloat->FloatCurve.Keys.Last().Time;
}


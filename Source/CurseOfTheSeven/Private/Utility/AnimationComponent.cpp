// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AnimationComponent.h"

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
	AnimatedActor->SetActorLocation(NewLocation);
}

void UAnimationComponent::Dash(AActor* Actor, FVector Offset)
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


		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Position x: %f, y: %f, z: %f"), AnimatedActor->GetActorLocation().X, AnimatedActor->GetActorLocation().Y, AnimatedActor->GetActorLocation().Z));
	}
}


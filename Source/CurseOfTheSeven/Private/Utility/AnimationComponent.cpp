// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/AnimationComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"

UAnimationComponent::UAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurveFloatTimeline.TickTimeline(DeltaTime);
	CurveCameraTimeline.TickTimeline(DeltaTime);
	CurveCameraPostProcessTimeline.TickTimeline(DeltaTime);
}

void UAnimationComponent::TimeLineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(StartPosition, EndPosition, Value);
	FHitResult* HitResult = nullptr;
	AnimatedActor->SetActorLocation(NewLocation, true, HitResult, ETeleportType::None);
}

void UAnimationComponent::CameraTimeLineProgress(FVector Value)
{
	DRAW_TEXT_ONSCREEN(Value.ToString());

	Camera->SetRelativeLocation(Value);

	DRAW_TEXT_ONSCREEN(TEXT("Movement"));

}

void UAnimationComponent::ProcessTimeLineProgress(FVector Value)
{
	DRAW_TEXT_ONSCREEN(Value.ToString());
	Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
	
	Camera->PostProcessSettings.DepthOfFieldFocalDistance = Value.Y;
	Camera->PostProcessSettings.DepthOfFieldSensorWidth = Value.Z;

	DRAW_TEXT_ONSCREEN(TEXT("PostProcess"));
}

void UAnimationComponent::Dash(ACharacter* Actor, FVector Offset)
{
	if (CurveFloat && !CurveFloatTimeline.IsPlaying())
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

void UAnimationComponent::CameraMovementAnimation(UCameraComponent* CameraActor)
{
	if (CameraCurvePosition && !CurveCameraTimeline.IsPlaying())
	{
		if (Camera == nullptr)
		{
			Camera = CameraActor;
		}

		FOnTimelineVector TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("CameraTimeLineProgress"));
		CurveCameraTimeline.AddInterpVector(CameraCurvePosition, TimeLineProgress);
		CurveCameraTimeline.SetLooping(false);
		CurveCameraTimeline.PlayFromStart();

		
		// FTimerHandle UnusedHandle;
		// AnimatedActor->GetWorldTimerManager().SetTimer(UnusedHandle,this, &UAnimationComponent::DashCallback, 1.f);

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Time %f"),));
	}
}

void UAnimationComponent::CameraRotationAnimation(UCameraComponent* CameraActor)
{
}

void UAnimationComponent::CameraPostProcessAnimation(UCameraComponent* CameraActor)
{
	if (PostProcessCurve && !CurveCameraPostProcessTimeline.IsPlaying())
	{
		if (Camera == nullptr)
		{
			Camera = CameraActor;
		}
		FOnTimelineVector TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("ProcessTimeLineProgress"));
		CurveCameraPostProcessTimeline.AddInterpVector(PostProcessCurve, TimeLineProgress);
		CurveCameraPostProcessTimeline.SetLooping(false);
		CurveCameraPostProcessTimeline.PlayFromStart();
		
	}
}

float UAnimationComponent::GetDashTime()
{
	return CurveFloat->FloatCurve.Keys.Last().Time;
}

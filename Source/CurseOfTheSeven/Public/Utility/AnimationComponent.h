// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "AnimationComponent.generated.h"
class UCurveFloat;
class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CURSEOFTHESEVEN_API UAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAnimationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TimeLineProgress(float Value);

	UFUNCTION()
	void CameraTimeLineProgress(FVector Value);

	UFUNCTION()
	void ProcessTimeLineProgress(FVector Value);

	UFUNCTION()
	void Dash(ACharacter* Actor, FVector Offset);

	UFUNCTION()
	void CameraMovementAnimation(UCameraComponent* CameraActor);

	UFUNCTION()
	void CameraRotationAnimation(UCameraComponent* CameraActor);

	UFUNCTION()
	void CameraPostProcessAnimation(UCameraComponent* CameraActor);

	UFUNCTION()
	float GetDashTime();

protected:
	virtual void BeginPlay() override;

	FTimeline CurveFloatTimeline;
	FTimeline CurveCameraTimeline;
	FTimeline CurveCameraPostProcessTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveVector* CameraCurvePosition;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveVector* CameraCurveRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	UCurveVector* PostProcessCurve;

	UPROPERTY()
	FVector StartPosition;

	UPROPERTY()
	FVector EndPosition;

private:
	UPROPERTY(VisibleAnywhere)
	ACharacter* AnimatedActor = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera = nullptr;
};

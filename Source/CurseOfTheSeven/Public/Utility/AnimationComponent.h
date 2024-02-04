// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "AnimationComponent.generated.h"
class UCurveFloat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTHESEVEN_API UAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAnimationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TimeLineProgress(float Value);

	UFUNCTION()
	void Dash(ACharacter* Actor, FVector Offset);

	UFUNCTION()
	float GetDashTime();

protected:
	virtual void BeginPlay() override;

	FTimeline CurveFloatTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	FVector StartPosition;

	UPROPERTY()
	FVector EndPosition;
	
private:
	UPROPERTY(VisibleAnywhere)
	ACharacter* AnimatedActor = nullptr;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "HittableActor.generated.h"

UCLASS()
class CURSEOFTHESEVEN_API AHittableActor : public AActor ,public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHittableActor();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* VisualFX;
	
	UPROPERTY(EditAnywhere)
	USoundBase* SoundFX;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

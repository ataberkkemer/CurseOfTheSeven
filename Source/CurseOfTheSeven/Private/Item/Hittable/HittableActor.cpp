// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Hittable/HittableActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHittableActor::AHittableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(ItemMesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	BoxComponent->SetupAttachment(GetRootComponent());

	
	VisualFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitParticle"));
	VisualFX->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AHittableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHittableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHittableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if(VisualFX)
	{
		VisualFX->SetWorldLocation(ImpactPoint);
		VisualFX->Activate(true);
	}
	if(SoundFX)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			SoundFX,
			GetActorLocation()
		);
	}
}


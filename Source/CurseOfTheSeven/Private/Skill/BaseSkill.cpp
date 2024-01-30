// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/BaseSkill.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/Components/SkillAttribiuteComponent.h"

ABaseSkill::ABaseSkill()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(Root);
	LifeSpan = 5.f;

	Attributes = CreateDefaultSubobject<USkillAttribiuteComponent>(TEXT("Attributes"));
}

void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	SkillIndex = 0;
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseSkill::OnSphereOverlap);
	SetLifeSpan(LifeSpan);
	if(ActiveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
					this,
					ActiveSound,
					GetActorLocation()
				);
	}
}

void ABaseSkill::SetAttributes()
{
	
}


void ABaseSkill::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != GetOwner())
	{
		DRAW_TEXT_ONSCREEN(SweepResult.ImpactPoint.ToString());
		SetLifeSpan(3.f);
		if (HitEffect && GetWorld())
		{
			DRAW_TEXT_ONSCREEN(SweepResult.GetActor()->GetName());
			HitParticleInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), HitEffect, SweepResult.ImpactPoint);

			HitParticleInstance->Activate();

			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					SweepResult.ImpactPoint
				);
			}
		}
	}
}


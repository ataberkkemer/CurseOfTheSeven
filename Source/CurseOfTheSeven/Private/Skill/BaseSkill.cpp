// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/BaseSkill.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/Components/SkillAttribiuteComponent.h"

ABaseSkill::ABaseSkill()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	LifeSpan = 5.f;

	Attributes = CreateDefaultSubobject<USkillAttribiuteComponent>(TEXT("Attributes"));
}

void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentHit.AddDynamic(this, &ABaseSkill::OnHit);
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

void ABaseSkill::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                       FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != GetOwner())
	{
		DRAW_TEXT_ONSCREEN(Hit.ImpactPoint.ToString());
		SetLifeSpan(1.f);
		if (HitEffect && GetWorld())
		{
			//TODO Despawn
			HitParticleInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), HitEffect, Hit.ImpactPoint);

			HitParticleInstance->Activate();

			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					Hit.ImpactPoint
				);
			}
		}
	}
}

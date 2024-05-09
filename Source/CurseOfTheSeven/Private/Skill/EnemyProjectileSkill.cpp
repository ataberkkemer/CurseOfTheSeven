// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/EnemyProjectileSkill.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/CHeroCharacter.h"
#include "Components/ArrowComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/Components/SkillAttribiuteComponent.h"

AEnemyProjectileSkill::AEnemyProjectileSkill()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"))->SetupAttachment(Root);

	SkillEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillParticle"));
	SkillEffect->SetupAttachment(Root);
	
	Speed = 500.f;
	MaxSpeed = 500.f;
	Gravity = 0.f;

	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = Gravity;
}

void AEnemyProjectileSkill::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != GetOwner())
	{
		DRAW_TEXT_ONSCREEN(OtherActor->GetName());
		
		if (OwnerActor)
		{
			SetOwner(OwnerActor);
			SetInstigator(InstigatorPawn);
		}
		if (ActorIsSameType(OtherActor)) return;
				ACHeroCharacter* Hero = Cast<ACHeroCharacter>(OtherActor);

		if (!Hero && HitEffect)
		{
			HitParticleInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), HitEffect, SweepResult.ImpactPoint);

			HitParticleInstance->Activate();
			DRAW_TEXT_ONSCREEN(HitParticleInstance->GetName());
			Destroy();
			return;
		}
		APawn* Ins = GetInstigator();

		if (!Ins)
		{
			return;
		}

		FHitResult SphereHit;
		SphereTrace(SphereHit);

		if (SphereHit.GetActor())
		{
			if (ActorIsSameType(SphereHit.GetActor())) return;

			UGameplayStatics::ApplyDamage(SphereHit.GetActor(), Attributes->GetRawDamage(), GetInstigator()->GetController(), this,
							  UDamageType::StaticClass());
			ExecuteGetHit(SphereHit);

			if(IsMultipleHitSkill)
			{
				FTimerHandle UniqueHandle;
				FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject( this, &ABaseSkill::ExecuteMultipleHitTry, SphereHit);
				GetWorldTimerManager().SetTimer( UniqueHandle, RespawnDelegate, 0.25f, true, false );
			}

			DRAW_TEXT_ONSCREEN(TEXT("Hit"));
			
			// CreateFields(SphereHit.ImpactPoint);
		}

		if (HitEffect && GetWorld())
		{
			HitParticleInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), HitEffect, SweepResult.ImpactPoint);

			HitParticleInstance->Activate();
			DRAW_TEXT_ONSCREEN(HitParticleInstance->GetName());
		}
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

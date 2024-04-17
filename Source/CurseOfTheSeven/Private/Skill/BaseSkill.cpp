// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/BaseSkill.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/CHeroCharacter.h"
#include "Character/Enemy/BaseEnemy.h"
#include "Components/SphereComponent.h"
#include "CurseOfTheSeven/DebugMacros.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Item/Breakable/BreakableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Skill/Components/SkillAttribiuteComponent.h"

ABaseSkill::ABaseSkill()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(Root);
	LifeSpan = 5.f;
	SphereTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Sphere Trace Start"));
	SphereTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Sphere Trace End"));

	SphereTraceStart->SetupAttachment(SphereCollision);
	SphereTraceEnd->SetupAttachment(SphereCollision);

	Attributes = CreateDefaultSubobject<USkillAttribiuteComponent>(TEXT("Attributes"));
}

void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	SkillIndex = 0;
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseSkill::OnSphereOverlap);
	// SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseSkill::OnSphereOverlapEnd);
	SetLifeSpan(LifeSpan);
	if (ActiveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ActiveSound,
			GetActorLocation()
		);
	}
}

void ABaseSkill::Equip(AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
}

void ABaseSkill::SetAttributes(float RawDamage, float ElementalDamage, float DamageTickInterval, float StaggerDamage)
{
	Attributes->SetRawDamage(RawDamage);
	Attributes->SetElementalDamage(ElementalDamage);
	Attributes->SetStaggerDamage(StaggerDamage);
}

void ABaseSkill::SphereTrace(FHitResult& SphereHit)
{
	const FVector Start = SphereTraceStart->GetComponentLocation();
	const FVector End = SphereTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		200.f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		SphereHit,
		true
	);

	IgnoreActors.AddUnique(SphereHit.GetActor());
}

void ABaseSkill::DisableActor(bool toHide) 
{
	// Hides visible components
	SetActorHiddenInGame(toHide);

	// Disables collision components
	SetActorEnableCollision(!toHide);

	// Stops the Actor from ticking
	SetActorTickEnabled(!toHide);
}

void ABaseSkill::ExecuteMultipleHitTry(FHitResult HitResult)
{
	UGameplayStatics::ApplyDamage(HitResult.GetActor(), 50, GetInstigator()->GetController(), this,
							  UDamageType::StaticClass());
	ExecuteGetHit(HitResult);
}

void ABaseSkill::ExecuteMultipleHit(FHitResult& SphereHit)
{
	UGameplayStatics::ApplyDamage(SphereHit.GetActor(), 50, GetInstigator()->GetController(), this,
							  UDamageType::StaticClass());
	ExecuteGetHit(SphereHit);
}

void ABaseSkill::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != GetOwner())
	{
		ACHeroCharacter* Hero = Cast<ACHeroCharacter>(OtherActor);
		if (Hero)
		{
			SetOwner(Hero);
			SetInstigator(Hero);
		}
		if (ActorIsSameType(OtherActor)) return;
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
		if (!Enemy)
		{
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

void ABaseSkill::OnSphereOverlapEnd()
{
}

bool ABaseSkill::ActorIsSameType(AActor* OtherActor)
{
	AActor* Ow = GetOwner();

	if(!Ow)
	{
		return false;
	}
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}
void ABaseSkill::ExecuteGetHit(FHitResult& SphereHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(SphereHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(SphereHit.GetActor(), SphereHit.ImpactPoint, GetOwner());
	}
}
void ABaseSkill::RespawnPlayerWithDelay(FHitResult Player)
{
	const float Interval = 3.f;
	const bool Loop = false;
	const float FirstDelay = 0.f;
	FTimerHandle UniqueHandle;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject( this, &ABaseSkill::ExecuteMultipleHitTry, Player );
	GetWorldTimerManager().SetTimer( UniqueHandle, RespawnDelegate, Interval, Loop, FirstDelay );
}

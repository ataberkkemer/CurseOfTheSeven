// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/ProjectileSkill.h"

#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileSkill::AProjectileSkill()
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

void AProjectileSkill::BeginPlay()
{
	Super::BeginPlay();
	
}



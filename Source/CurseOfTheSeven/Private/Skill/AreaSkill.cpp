// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/AreaSkill.h"
#include "NiagaraComponent.h"

AAreaSkill::AAreaSkill()
{
	SkillEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillParticle"));
	SkillEffect->SetupAttachment(Root);
}

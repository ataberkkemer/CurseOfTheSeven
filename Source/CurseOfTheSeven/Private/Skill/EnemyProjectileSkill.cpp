// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/EnemyProjectileSkill.h"

#include "Character/CHeroCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEnemyProjectileSkill::AEnemyProjectileSkill()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
}

void AEnemyProjectileSkill::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		ACHeroCharacter* HeroCharacter = Cast<ACHeroCharacter>(OtherActor);

		if(HeroCharacter)
		{
			Destroy();
		}
	}
	
}

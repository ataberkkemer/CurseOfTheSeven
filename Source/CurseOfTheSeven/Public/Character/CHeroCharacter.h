// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "CHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkillSlotComponent;
class UInputMappingContext;
class UInputAction;
class UNiagaraComponent;
class ULegacyCameraShake;
class AItem;
class AWeapon;
class UAnimMontage;
class UAnimationComponent;
struct FInputActionValue;


UCLASS()
class CURSEOFTHESEVEN_API ACHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()


public:
	ACHeroCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Getter Functions
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE bool IsCharacterDashing() const { return IsDashing; }
	
	//Setter Functions
	FORCEINLINE void SetOverlapingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE void SetCharacterState(AItem* Item) { OverlappingItem = Item; }

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	void Move(const FInputActionValue& Value);
	void Dash();
	void ResetDash();

	UFUNCTION()
	void Equip();
	virtual void Attack() override;
	// virtual void PlayAttackMontage() override;
	// virtual void AttackEnd() override;
	// virtual bool CanAttack() override;


	UFUNCTION(BlueprintCallable)
	void SaveCharacterStatus();
	
	UFUNCTION(BlueprintCallable)
	void LoadCharacterStatus();
	
	void SaveDelegate(const FString& SlotName, int UserIndex, bool IsSuccess);
	
	//FirstSkill
	void CastFirstSkill();
	void SpawnFirstSkill();

	//SecondSkill
	void CastSecondSkill();
	void SpawnSecondSkill();

	//UltimateSkill
	void CastUltimateSkill();
	void SpawnUltimateSkill();
	void CheckAttack();

	virtual void BeginPlay() override;

private:
	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;
	int32 AttackState = 1;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULegacyCameraShake> CameraShake;
	
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* DashVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* SecondaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipKeyAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FirstSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SecondSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UltimateSkillAction;

	/// Components
	UPROPERTY(VisibleAnywhere)
	UAnimationComponent* AnimationComponent;

	UPROPERTY(VisibleAnywhere)
	USkillSlotComponent* FirstSkillSlotComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkillSlotComponent* SecondSkillSlotComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkillSlotComponent* UltimateSkillSlotComponent;

	UPROPERTY()
	FVector2D MovementVector;
	
	bool IsDashing;

	void ShakeCamera();
	float GetMovementAngle();
};

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
	void SecondaryAttack();
	void KeyboardKeyPressed();
	void GamepadKeyPressed();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Getter Functions
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE bool IsCharacterDashing() const { return IsDashing; }
	FORCEINLINE bool IsCharacterAttacking() const { return (IsDashing && IsAttacking); }
	
	//Setter Functions
	FORCEINLINE void SetOverlapingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE void SetCharacterState(AItem* Item) { OverlappingItem = Item; }

	void ResetStagger();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	UFUNCTION(BlueprintCallable)
	void CameraMovementAnimation();

	UFUNCTION(BlueprintCallable)
	void CameraPostProcessAnimation();

protected:
	void Move(const FInputActionValue& Value);
	void Dash();
	void ResetDash();

	UFUNCTION()
	void Equip();
	virtual void Attack() override;

	void ResetAttack();
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

	
	UPROPERTY(BlueprintReadOnly)
	bool IsDashing;
	
	UPROPERTY(BlueprintReadOnly)
	bool IsAttacking;
	
	FTimerHandle AttackHandle;
	UPROPERTY(VisibleAnywhere)
	int32 AttackState = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsStagger = false;
	FTimerHandle StaggerHandle;
	float Timer;

private:
	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;

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
    UInputAction* AttackAction;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* KeyboardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GamepadAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SpecialAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashLimit;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float CurrentDashCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashRechargeRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, meta = (AllowPrivateAccess = "true"))
	float DashTimer;
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
	
	
	void ShakeCamera();
	float GetMovementAngle();
	void StartDashTimer(float DeltaTime);

	bool IsKeyboard = false;
};

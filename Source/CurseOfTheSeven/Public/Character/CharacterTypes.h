
#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "Unequipped"),
	ECS_EquipOneHandedSword UMETA(DisplayName = "Equiped One-Handed Sword"),
	ECS_EquipTwoHandedSword UMETA(DisplayName = "Equiped Two-Handed Sword")
};

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	ECMS_Idle UMETA(DisplayName = "Idle"),
	ECMS_Running UMETA(DisplayName = "Running"),
	ECMS_Dashing UMETA(DisplayName = "Dashing"),
	ECMS_Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_DeathA UMETA(DisplayName = "Death_A"),
	EDP_DeathB UMETA(DisplayName = "Death_B")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Stagger UMETA(DisplayName = "Stagger")
};
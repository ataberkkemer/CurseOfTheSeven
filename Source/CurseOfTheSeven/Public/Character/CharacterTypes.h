#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "Unequipped") = 0 ,
	ECS_EquipOneHandedSword UMETA(DisplayName = "Equiped One-Handed Sword"),
	ECS_EquipTwoHandedSword UMETA(DisplayName = "Equiped Two-Handed Sword")
};
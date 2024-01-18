#pragma once

UENUM(BlueprintType)
enum class UpgradeTypes: uint8
{
	UT_Damage UMETA(DisplayName = "Damage Upgrade"),
	UT_Size UMETA(DisplayName = "Size Upgrade"),
	UT_DoubleCasting UMETA(DisplayName = "Double Casting")
};
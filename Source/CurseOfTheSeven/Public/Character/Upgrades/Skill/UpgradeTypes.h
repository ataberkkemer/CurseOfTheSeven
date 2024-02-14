#pragma once

UENUM(BlueprintType)
enum class UpgradeTypes: uint8
{
	UT_UniqueUpgrade UMETA(DisplayName = "Unique Upgrade"),
	UT_Damage UMETA(DisplayName = "Damage Upgrade"),
};
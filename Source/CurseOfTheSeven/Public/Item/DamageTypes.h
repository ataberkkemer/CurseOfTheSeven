#pragma once

UENUM(BlueprintType)
enum class ElementType: uint8
{
	ET_Fire UMETA(DisplayName = "Fire"),
	ET_Poison UMETA(DisplayName = "Poison"),
	ET_Lightning UMETA(DisplayName = "Lightning")
};
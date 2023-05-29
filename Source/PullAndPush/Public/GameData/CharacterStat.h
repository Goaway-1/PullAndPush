#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterStat : uint8
{
	None = 0 UMETA(Hidden),
	Stun = 1 << 0 UMETA(DisplayName = "Stun"),
	Snare = 1 << 1 UMETA(DisplayName = "Snare"),
	Slow = 1 << 2 UMETA(DisplayName = "Slow")
};
ENUM_CLASS_FLAGS(ECharacterStat);
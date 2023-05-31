#pragma once

#include "CoreMinimal.h"
#include "CharacterStat.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterStat : uint8
{
	None = 0 UMETA(Hidden),
	Stun = 1 << 0 UMETA(DisplayName = "Stun"),
	Snare = 1 << 1 UMETA(DisplayName = "Snare"),
	Slow = 1 << 2 UMETA(DisplayName = "Slow")
};
ENUM_CLASS_FLAGS(ECharacterStat);

/** 
* Data structures to assign CharacterStat 
* It is used to change the status of the character
*/
USTRUCT(BlueprintType)
struct FCharacterStatModifier
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	ECharacterStat Stat;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float ChangeDuration;
};
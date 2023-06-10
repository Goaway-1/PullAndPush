#pragma once

#include "CoreMinimal.h"
#include "CharacterStat.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterStat : uint8
{
	None = 0 UMETA(Hidden),
	Stun = 0x01 UMETA(DisplayName = "Stun"),
	Snare = 0x02 UMETA(DisplayName = "Snare"),
	Slow = 0x04 UMETA(DisplayName = "Slow")
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
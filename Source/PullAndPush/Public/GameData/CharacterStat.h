#pragma once

#include "CoreMinimal.h"
#include "CharacterStat.generated.h"

// 캐릭터의 공격과 관련된...
USTRUCT(BlueprintType)
struct FItemEnhancedStat
{
	GENERATED_BODY()

public:
	FItemEnhancedStat() : RPSpeed(0.f), RPScale(0.f), RPRange(0.f), RPForce(0.f), RPStun(0) {}

	UPROPERTY(EditDefaultsOnly)
	float RPSpeed;

	UPROPERTY(EditDefaultsOnly)
	float RPScale;

	UPROPERTY(EditDefaultsOnly)
	float RPRange;

	UPROPERTY(EditDefaultsOnly)
	float RPForce;

	UPROPERTY(EditDefaultsOnly)
	uint8 RPStun:1;

	FItemEnhancedStat operator+(const FItemEnhancedStat& Other) const
	{
		FItemEnhancedStat Result;
		Result.RPSpeed = RPSpeed + Other.RPSpeed;
		Result.RPScale = RPScale + Other.RPScale;
		Result.RPRange = RPRange + Other.RPRange;
		Result.RPForce = RPForce + Other.RPForce;
		Result.RPStun = Other.RPStun;

		return Result;
	}

	FItemEnhancedStat operator*(const float Other) const
	{
		FItemEnhancedStat Result;
		Result.RPSpeed = RPSpeed * Other;
		Result.RPScale = RPScale * Other;
		Result.RPRange = RPRange * Other;
		Result.RPForce = RPForce * Other;
		Result.RPStun = 0;

		return Result;
	}
};

// 캐릭터의 현재 상태를 나타낸...
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterStat : uint8
{
	None = 0			UMETA(Hidden),
	Stun = 0x01			UMETA(DisplayName = "Stun"),
	Snare = 0x02		UMETA(DisplayName = "Snare"),
	Slow = 0x04			UMETA(DisplayName = "Slow"),
	Immunity = 0x10		UMETA(DisplayName = "Immunity")
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
	FCharacterStatModifier() : Stat(ECharacterStat::None), ChangeDuration(0.f) {}

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	ECharacterStat Stat;

	UPROPERTY(EditDefaultsOnly, Category = Stat)
	float ChangeDuration;
};
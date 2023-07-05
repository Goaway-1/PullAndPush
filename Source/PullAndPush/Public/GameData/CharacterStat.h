#pragma once

#include "CoreMinimal.h"
#include "CharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FPassiveStat
{
	GENERATED_BODY()

public:
	FPassiveStat() : RPSpeed(0.f), RPScale(0.f), RPRange(0.f), RPForce(0.f){}

	UPROPERTY(EditDefaultsOnly)
	float RPSpeed;

	UPROPERTY(EditDefaultsOnly)
	float RPScale;

	UPROPERTY(EditDefaultsOnly)
	float RPRange;

	UPROPERTY(EditDefaultsOnly)
	float RPForce;

	FPassiveStat operator+(const FPassiveStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FPassiveStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FPassiveStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}

	FPassiveStat operator*(const float Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FPassiveStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FPassiveStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] * Other;
		}

		return Result;
	}
};

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
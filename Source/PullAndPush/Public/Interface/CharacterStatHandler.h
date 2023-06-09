// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/CharacterStat.h"
#include "CharacterStatHandler.generated.h"

UENUM(BlueprintType)
enum class EPlayerAttackCondition : uint8 {
	EPAC_Default = 0		UMETA(DisplayName = "Default"),
	EPAC_Idle				UMETA(DisplayName = "Idle"),
	EPAC_Charging			UMETA(DisplayName = "Charging")
};

UINTERFACE(MinimalAPI)
class UCharacterStatHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * Set PlayerCharacter Properties...
 * like speed, state..
 */
class PULLANDPUSH_API ICharacterStatHandler
{
	GENERATED_BODY()

public:
	/**
	* 아이템 습득 : 모두 지정
	* 주무기 차징 : IsCharging만 지정
	*/
	virtual void SetPlayerAttackCondition(const bool IsCharging) = 0;

public:
	/** Item : Character Movement Speed Up 
	* Not Only Items, Also Calls When Rocket Punch Charging...
	*/
	UFUNCTION()
	virtual void SetMovementSpeed(const float NewMoveSpeed = 0.f) = 0;

	/** Item : For Passive Stat */
	UFUNCTION()
	virtual void SetPassiveStat(FPassiveStat InPassiveStat) = 0;

	UFUNCTION()
	virtual FPassiveStat GetPassiveStat() = 0;

/** Character Stat */
public:
	UFUNCTION()
	virtual void EnableStatFlag(ECharacterStat InFlag, float ChangeDuration) = 0;

	UFUNCTION()
	virtual void DisableStatFlag(ECharacterStat InFlag) = 0;

	UFUNCTION()
	virtual bool IsStatFlagSet(ECharacterStat InFlag) = 0;	// Is Flag bit enable?

};

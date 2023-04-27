// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterPropertyHandler.generated.h"

UENUM(BlueprintType)
enum class EPlayerAttackCondition : uint8 {
	EPAC_Default = 0		UMETA(DisplayName = "Default"),
	EPAC_Idle				UMETA(DisplayName = "Idle"),
	EPAC_Charging			UMETA(DisplayName = "Charging")
};


UINTERFACE(MinimalAPI)
class UCharacterPropertyHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * Set PlayerCharacter Properties...
 * like speed, state..
 */
class PULLANDPUSH_API ICharacterPropertyHandler
{
	GENERATED_BODY()

public:
	/**
	* 아이템 습득 : 모두 지정
	* 주무기 차징 : IsCharging만 지정
	*/
	UFUNCTION()
	virtual void SetMovementSpeed(const bool& IsCharging, const float& NewMoveSpeed = 0.f) = 0;

	virtual void SetPlayerAttackCondition(const bool& IsCharging) = 0;
};

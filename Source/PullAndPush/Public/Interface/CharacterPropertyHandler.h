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
	* ������ ���� : ��� ����
	* �ֹ��� ��¡ : IsCharging�� ����
	*/
	virtual void SetPlayerAttackCondition(const bool IsCharging) = 0;

#pragma region ITEM
/** Item : Character Movement Speed Up 
* Not Only Items, Also Calls When Rocket Punch Charging...
*/
	UFUNCTION()
	virtual void SetMovementSpeed(const bool& IsCharging, const float& NewMoveSpeed = 0.f) = 0;

/** Item : For Rocket Punch */
#pragma endregion
	UFUNCTION()
	virtual void RocketPunchAlphaSpeed(const float& AlphaSpeed) = 0;

	UFUNCTION()
	virtual void RocketPunchAlphaRange(const float& AlphaRange) = 0;

	UFUNCTION()
	virtual void RocketPunchAlphaSize(const float& AlphaSize) = 0;
};

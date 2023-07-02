 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackWeapon.generated.h"

DECLARE_DELEGATE_OneParam(FAttackWeaponOutOfUse, const bool&)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackWeapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * ĳ���Ͱ� �߻��� �� �ִ� ����ü Ŭ����.
 */
class PULLANDPUSH_API IAttackWeapon
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void ReadyToLaunch(const float& InForceAlpha, AActor* InOwnerPlayerActor, const bool IsPush, const FVector& InVec, const FRotator& InRot) = 0;

	UFUNCTION()
	virtual AActor* GetCasterActor() = 0;

	UFUNCTION()
	virtual void IsOutOfUse(const bool& Val) = 0;

	FAttackWeaponOutOfUse OutOfUse;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData/PassiveItemData/PassiveItemData.h"
#include "PassiveItemDataRPSpeedUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemDataRPSpeedUp : public UPassiveItemData
{
	GENERATED_BODY()

public:
	UPassiveItemDataRPSpeedUp() {
		Name = "RP_SpeedUp";
		Description = "Rocket Punch Speed Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 2.0f;
	}

/** Handler */
public:
	virtual void UseItem(class AActor* TargetActor) override;
	virtual void EndActiveItem() override;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PassiveItem/PassiveItem.h"
#include "PassiveItemRPSpeedUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemRPSpeedUp : public UPassiveItem
{
	GENERATED_BODY()

/** Handler */
public:
	UPassiveItemRPSpeedUp() {
		Name = "RP_SpeedUp";
		Description = "Rocket Punch Speed Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 2.0f;
	}

	virtual void UseItem(class AActor* TargetActor) override;

	virtual void EndActiveItem() override;
};

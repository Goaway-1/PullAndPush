// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PassiveItem/PassiveItem.h"
#include "PassiveItemRPRangeUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemRPRangeUp : public UPassiveItem
{
	GENERATED_BODY()

/** Handler */
public:
	UPassiveItemRPRangeUp() {
		Name = "RP_RangeUp";
		Description = "Rocket Punch Range Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 2.0f;
	}

	virtual void UseItem(class AActor* TargetActor) override;

	virtual void EndActiveItem() override;
};

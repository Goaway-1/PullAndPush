// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData/PassiveItemData/PassiveItemData.h"
#include "PassiveItemDataRPRangeUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemDataRPRangeUp : public UPassiveItemData
{
	GENERATED_BODY()

public:
	UPassiveItemDataRPRangeUp() {
		Name = "RP_RangeUp";
		Description = "Rocket Punch Range Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 2.0f;
	}

/** Handler */
public:
	virtual void UsePassiveItem(class AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated) override;
	virtual void EndPassiveItem() override;
};

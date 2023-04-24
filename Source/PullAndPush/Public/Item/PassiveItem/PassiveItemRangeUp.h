// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PassiveItem/PassiveItem.h"
#include "PassiveItemRangeUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemRangeUp : public UPassiveItem
{
	GENERATED_BODY()

/** Handler */
public:
	UPassiveItemRangeUp() {
		Name = "RangeUp";
		Description = "Rocket Punch Range Up for 3sec!";
		DurationTime = 3.f;
		WeightValue = 400.f;
	}

	virtual void UseItem(class APlayableCharacter* TargetCharacter) override;

	virtual void EndActiveItem() override;
};

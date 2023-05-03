// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PassiveItem/PassiveItem.h"
#include "PassiveItemSpeedUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemSpeedUp : public UPassiveItem
{
	GENERATED_BODY()

/** Handler */
public:
	UPassiveItemSpeedUp() {
		Name = "SpeedUp";
		Description = "Character Movement Speed Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 300.f;
	}

	virtual void UseItem(class AActor* TargetActor) override;

	virtual void EndActiveItem() override;
};

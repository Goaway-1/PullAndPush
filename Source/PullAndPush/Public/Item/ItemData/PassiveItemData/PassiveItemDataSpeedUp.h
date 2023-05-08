// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData/PassiveItemData/PassiveItemData.h"
#include "PassiveItemDataSpeedUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemDataSpeedUp : public UPassiveItemData
{
	GENERATED_BODY()

public:
	UPassiveItemDataSpeedUp() {
		Name = "SpeedUp";
		Description = "Character Movement Speed Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 300.f;
	}

/** Handler */
public:
	virtual void UseItem(class AActor* TargetActor) override;
	virtual void EndActiveItem() override;
};

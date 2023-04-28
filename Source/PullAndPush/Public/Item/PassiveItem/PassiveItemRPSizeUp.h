// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PassiveItem/PassiveItem.h"
#include "PassiveItemRPSizeUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemRPSizeUp : public UPassiveItem
{
	GENERATED_BODY()

	/** Handler */
public:
	UPassiveItemRPSizeUp() {
		Name = "RP_SizeUp";
		Description = "Rocket Punch Size Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 2.f;	
	}

	virtual void UseItem(class AActor* TargetActor) override;

	virtual void EndActiveItem() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData/PassiveItemData/PassiveItemData.h"
#include "PassiveItemDataRPSizeUp.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UPassiveItemDataRPSizeUp : public UPassiveItemData
{
	GENERATED_BODY()

public:
	UPassiveItemDataRPSizeUp() {
		Name = "RP_SizeUp";
		Description = "Rocket Punch Size Up for 5sec!";
		DurationTime = 5.f;
		WeightValue = 2.f;	
	}

/** Handler */
public:
	virtual void UsePassiveItem(class AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated) override;
	virtual void EndPassiveItem() override;
};

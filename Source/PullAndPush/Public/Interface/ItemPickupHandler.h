// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemPickupHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemPickupHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PULLANDPUSH_API IItemPickupHandler
{
	GENERATED_BODY()

public:
	// Settings when an item is created or used
	UFUNCTION()
	virtual void SetActiveItemPickup(bool IsSpawn, class UItemData* InItemDataAsset = nullptr, FVector InSpawnLocation = FVector(0.f)) = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemActionHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemActionHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * Item Action... 
 */
class PULLANDPUSH_API IItemActionHandler
{
	GENERATED_BODY()

public:
	virtual FString GetIdentifierString() const = 0;
	virtual bool CheckIsActiveItem() const = 0;
	virtual UStaticMesh* GetStaticMesh() const = 0;

	// For UI
	virtual FString GetItemName() = 0;
	virtual FString GetItemDescription() = 0;
	virtual UMaterialInterface* GetItemMaterialInterface() = 0;

	// For Passive
	UFUNCTION()
	virtual void UsePassiveItem(class AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated) = 0;
	virtual void EndPassiveItem() = 0;								
	virtual float GetDurationTime() = 0;
	virtual FTimerHandle GetTimerHandler() = 0;

	// For Active
	virtual TSubclassOf<AActor> GetSpawnItemClass() const = 0;
};

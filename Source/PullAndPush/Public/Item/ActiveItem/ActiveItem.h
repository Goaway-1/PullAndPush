// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/Item.h"
#include "ActiveItem.generated.h"

UCLASS()
class PULLANDPUSH_API UActiveItem : public UItem
{
	GENERATED_BODY()

public:
	UActiveItem()
	{
		ItemType = UItemAssetManager::ActiveItemType;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<class AActor> DeployableItemClass;

/** Handler */
public:
	virtual TSubclassOf<AActor> GetSpawnItemClass() const override {
		return DeployableItemClass;
	}
};

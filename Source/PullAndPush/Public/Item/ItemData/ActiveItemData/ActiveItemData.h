// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/ItemData/ItemData.h"
#include "ActiveItemData.generated.h"

UCLASS()
class PULLANDPUSH_API UActiveItemData : public UItemData
{
	GENERATED_BODY()

public:
	UActiveItemData()
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

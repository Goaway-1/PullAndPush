// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/Item.h"
#include "PassiveItem.generated.h"

UCLASS()
class PULLANDPUSH_API UPassiveItem : public UItem
{
	GENERATED_BODY()

public:
	UPassiveItem()
	{
		ItemType = UItemAssetManager::PassiveItemType;
	}

	/** For Passive Item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float DurationTime;				

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float WeightValue;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Interface/CharacterPropertyHandler.h"
#include "PassiveItem.generated.h"

UCLASS()
class PULLANDPUSH_API UPassiveItem : public UItem
{
	GENERATED_BODY()

public:
	UPassiveItem()
	{
		ItemType = UItemAssetManager::PassiveItemType;
		bIsItemActivated = false;
		CharacterPropertyHandler = nullptr;
	}

	/** For Passive Item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float DurationTime;				

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float WeightValue;

/** Handler */
	virtual void UseItem(class AActor* TargetActor) override;

	UFUNCTION()
	virtual void EndActiveItem();

protected:
	// Check Item Already Activeted
	uint8 bIsItemActivated;

	UPROPERTY()
	TScriptInterface<class ICharacterPropertyHandler> CharacterPropertyHandler;
};

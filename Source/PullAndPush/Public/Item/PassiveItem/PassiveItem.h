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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float DurationTime;				

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float WeightValue;

/** Handler */
public:
	virtual void UseItem(class AActor* TargetActor) override;

	UFUNCTION()
	virtual void EndActiveItem();

	virtual float GetDurationTime() override {
		return DurationTime;
	}
	virtual FTimerHandle GetTimerHandler() override {
		return ItemTimerHandler;
	}

protected:
	// Check Item Already Activeted
	uint8 bIsItemActivated : 1;

	UPROPERTY()
	TScriptInterface<class ICharacterPropertyHandler> CharacterPropertyHandler;

	// Use with ItemWidget
	FTimerHandle ItemTimerHandler;
};

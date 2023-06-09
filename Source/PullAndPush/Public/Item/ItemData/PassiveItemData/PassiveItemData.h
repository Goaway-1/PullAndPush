// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData/ItemData.h"
#include "Interface/CharacterStatHandler.h"
#include "PassiveItemData.generated.h"

UCLASS()
class PULLANDPUSH_API UPassiveItemData : public UItemData
{
	GENERATED_BODY()

public:
	UPassiveItemData()
	{
		ItemType = UItemAssetManager::PassiveItemType;
		bIsItemActivated = false;
		CharacterPropertyHandler = nullptr;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float DurationTime;	

/** Handler */
public:
	virtual void UsePassiveItem(class AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated) override;
	virtual void EndPassiveItem() override;

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
	TScriptInterface<class ICharacterStatHandler> CharacterPropertyHandler;

	// Use with ItemWidget
	FTimerHandle ItemTimerHandler;
};

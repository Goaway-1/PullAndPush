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

	/** For Active Item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	class UTexture2D* InterfaceUI;

	// @TODO : 투사체 Item으로 변경
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSubclassOf<class AActor> ProjectileItemClass;

/** Handler */
	virtual void UseItem(class APlayableCharacter* TargetCharacter) override;
};

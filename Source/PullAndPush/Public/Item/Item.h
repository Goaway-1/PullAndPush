// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Engine/DataAsset.h"
#include "Item/ItemAssetManager.h"
#include "Interface/ItemActionHandler.h"
#include "Item.generated.h"

UCLASS()
class PULLANDPUSH_API UItem : public UDataAsset, public IItemActionHandler
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	class UStaticMesh* StaticMesh;
	
	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	UFUNCTION(BlueprintCallable, Category = Item)
	bool CheckIsActiveItem() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

/** Handler */
	virtual void UseItem(class APlayableCharacter* TargetCharacter) override;
};

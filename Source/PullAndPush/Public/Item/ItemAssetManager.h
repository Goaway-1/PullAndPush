// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ItemAssetManager.generated.h"


UCLASS()
class PULLANDPUSH_API UItemAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UItemAssetManager() {}

	/** Static types for items */
	static const FPrimaryAssetType PassiveItemType;
	static const FPrimaryAssetType ActiveItemType;

	static UItemAssetManager& Get();

	class UItemData* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};

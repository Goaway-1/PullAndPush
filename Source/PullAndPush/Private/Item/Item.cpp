// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Item.h"

FString UItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

bool UItem::CheckIsActiveItem() const
{
	if (ItemType.IsValid() && ItemType == UItemAssetManager::ActiveItemType)
	{
		return true;
	}
	else {
		return false;
	}
}

FPrimaryAssetId UItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
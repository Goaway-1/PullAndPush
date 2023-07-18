// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ItemData/ItemData.h"

FString UItemData::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

bool UItemData::CheckIsActiveItem() const
{
	if (ItemType.IsValid() && ItemType == UItemAssetManager::ActiveItemType)
	{
		return true;
	}
	else {
		return false;
	}
}
FPrimaryAssetId UItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
UNiagaraSystem* UItemData::GetNiagraAsset() const
{
	if (NiagraAsset.IsPending())
	{
		NiagraAsset.LoadSynchronous();
	}
	return NiagraAsset.Get();
}
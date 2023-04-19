// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Item.h"

FString UItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
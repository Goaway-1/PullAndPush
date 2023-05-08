// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemAssetManager.h"
#include "Item/ItemData/ItemData.h"

const FPrimaryAssetType	UItemAssetManager::PassiveItemType = TEXT("PassiveItemData");
const FPrimaryAssetType	UItemAssetManager::ActiveItemType = TEXT("ActiveItemData");

UItemAssetManager& UItemAssetManager::Get()
{
	UItemAssetManager* This = Cast<UItemAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid AssetManager in DefaultEngine.ini, must be ItemAssetManager!"));
		return *NewObject<UItemAssetManager>(); // never calls this
	}
}
UItemData* UItemAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UItemData* LoadedItem = Cast<UItemData>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
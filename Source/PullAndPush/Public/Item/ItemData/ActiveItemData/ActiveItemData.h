// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/ItemData/ItemData.h"
#include "ActiveItemData.generated.h"

UCLASS()
class PULLANDPUSH_API UActiveItemData : public UItemData
{
	GENERATED_BODY()

public:
	UActiveItemData()
	{
		ItemType = UItemAssetManager::ActiveItemType;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<class AActor> DeployableItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<class UStaticMesh> StaticMesh;

/** Handler */
public:
	virtual TSubclassOf<AActor> GetDeployItemClass() const override {
		return DeployableItemClass;
	}
	virtual UStaticMesh* GetDeployStaticMesh() const override
	{
		if (StaticMesh.IsPending())
		{
			StaticMesh.LoadSynchronous();
		}
		return StaticMesh.Get();
	}
};

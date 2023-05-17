// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Engine/DataAsset.h"
#include "Item/ItemAssetManager.h"
#include "Interface/ItemActionHandler.h"
#include "ItemData.generated.h"

UCLASS()
class PULLANDPUSH_API UItemData : public UDataAsset, public IItemActionHandler
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSoftObjectPtr<UStaticMesh> StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSoftObjectPtr<UMaterialInterface> MaterialUI;

public:
	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UStaticMesh* GetStaticMesh();
public:
/** Handler */
	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable)
	virtual FString GetIdentifierString() const override;

	UFUNCTION(BlueprintCallable)
	virtual bool CheckIsActiveItem() const override;

	virtual void UseItem(class AActor* TargetActor) override {
		return;
	}
	virtual FString GetItemName() override {
		return Name;
	}
	virtual FString GetItemDescription() override {
		return Description;
	}
	virtual UMaterialInterface* GetItemMaterialInterface() override {
		if (MaterialUI.IsPending())
		{
			MaterialUI.LoadSynchronous();
		}
		return MaterialUI.Get();
	}
	virtual float GetDurationTime() override {
		return 0.f;
	}
	virtual FTimerHandle GetTimerHandler() override {
		return FTimerHandle();
	}
	virtual TSubclassOf<AActor> GetSpawnItemClass() const override {
		return nullptr;
	}
};

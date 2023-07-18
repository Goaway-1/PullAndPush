// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Engine/DataAsset.h"
#include "Item/ItemAssetManager.h"
#include "Interface/ItemDataHandler.h"
#include "ItemData.generated.h"

UCLASS()
class PULLANDPUSH_API UItemData : public UDataAsset, public IItemDataHandler
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
	TSoftObjectPtr<class UNiagaraSystem> NiagraAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSoftObjectPtr<UMaterialInterface> MaterialUI;

public:
	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual UNiagaraSystem* GetNiagraAsset()const override;
public:
/** Handler */
	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable)
	virtual FString GetIdentifierString() const override;

	UFUNCTION(BlueprintCallable)
	virtual bool CheckIsActiveItem() const override;

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
		if(CheckIsActiveItem()) PPLOG(Warning,TEXT("Invalid access to the Active item"));
		return 0.f;
	}
	virtual FTimerHandle GetTimerHandler() override {
		if (CheckIsActiveItem()) PPLOG(Warning, TEXT("Invalid access to the Active item"));
		return FTimerHandle();
	}
	virtual void UsePassiveItem(class AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated) override {
		if (CheckIsActiveItem()) PPLOG(Warning, TEXT("Invalid access to the Active item"));
		return;
	}
	virtual void EndPassiveItem() override {
		if (CheckIsActiveItem()) PPLOG(Warning, TEXT("Invalid access to the Active item"));
		return;
	}
	virtual TSubclassOf<AActor> GetDeployItemClass() const override {
		if (!CheckIsActiveItem()) PPLOG(Warning, TEXT("Invalid access to the Passive item"));
		return nullptr;
	}
	virtual UStaticMesh* GetDeployStaticMesh() const override
	{
		if (!CheckIsActiveItem()) PPLOG(Warning, TEXT("Invalid access to the Passive item"));
		return nullptr;
	}
};

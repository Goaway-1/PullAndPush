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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	class UStaticMesh* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	class UMaterialInterface* MaterialUI;

public:
	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	FORCEINLINE UStaticMesh* GetStaticMesh() const { return StaticMesh; }
public:
/** Handler */
	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable)
	virtual FString GetIdentifierString() const override;

	UFUNCTION(BlueprintCallable)
	virtual bool CheckIsActiveItem() const override;

	virtual void UseItem(class AActor* TargetActor) override {
		
	}
	virtual FString GetItemName() override {
		return Name;
	}
	virtual FString GetItemDescription() override {
		return Description;
	}
	virtual UMaterialInterface* GetItemMaterialInterface() override {
		return MaterialUI;
	}
	virtual float GetDurationTime() override {
		return 0.f;
	}
	virtual FTimerHandle GetTimerHandler() override {
		return FTimerHandle();
	}
};

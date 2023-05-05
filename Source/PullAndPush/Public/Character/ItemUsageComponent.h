// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Components/ActorComponent.h"
#include "Interface/PickupActionHandler.h"
#include "ItemUsageComponent.generated.h"

/**
* Update Item Widget..
* @param	DataAsset		Item Data Asset
* @param	bool			Whether the update target is Passive or Active (Passive is True)
*/
DECLARE_DELEGATE_TwoParams(FOnItemWidgetUpdate, class UDataAsset*, const bool&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API UItemUsageComponent : public UActorComponent, public IPickupActionHandler
{
	GENERATED_BODY()

public:	
	UItemUsageComponent();

/** Item */
public:
	void PickUpItem(class UItem* ItemData);
	void TryToUsePassiveItem(class UItem* ItemData);
	void TryToUseActiveItem();
	void ThrowDeployableItem();

	FORCEINLINE bool GetIsReadyToThrow() {return bIsReadyToThrow;}

private:
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UItem> CurActiveItemData;

	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class AActor> CurDeployableItem;

	const FName ItemSocketName = "ItemSocket";

	uint8 bIsReadyToThrow:1;

/** Widget */
public:
	FOnItemWidgetUpdate& GetItemWidgetUpdateDelegate() { return OnItemWidgetUpdate; }

private:
	/**
	* Update Item Widget..
	* @param	DataAsset		Item Data Asset
	* @param	bool			Whether the update target is Passive or Active (Passive is True)
	*/
	FOnItemWidgetUpdate OnItemWidgetUpdate;
};

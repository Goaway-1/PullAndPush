// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Components/ActorComponent.h"
#include "Interface/PickupActionHandler.h"
#include "ItemUsageComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnPassiveUIUpdate, class UDataAsset*)

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
	FOnPassiveUIUpdate& GetUpdatePassiveUI() { return OnUpdatePassiveUI; }

private:
	// Update PassiveUI
	FOnPassiveUIUpdate OnUpdatePassiveUI;
};

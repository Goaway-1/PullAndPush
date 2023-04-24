// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Components/ActorComponent.h"
#include "Interface/PickupActionHandler.h"
#include "Item/Item.h"
#include "ItemUsageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API UItemUsageComponent : public UActorComponent, public IPickupActionHandler
{
	GENERATED_BODY()

public:	
	UItemUsageComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

/** Item */
public:
	void PickUpItem(class UItem* ItemData);

	void StartActiveItem();
	void StartPassiveItem();
private:
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UItem> CurItemData;

};

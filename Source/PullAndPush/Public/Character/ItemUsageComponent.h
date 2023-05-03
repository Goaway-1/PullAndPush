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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

/** Item */
public:
	void PickUpItem(class UItem* ItemData);

	void StartActiveItem();
	void StartPassiveItem();

	FOnPassiveUIUpdate& GetUpdatePassiveUI() {return OnUpdatePassiveUI;}
private:
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UItem> CurItemData;

	// Update PassiveUI
	FOnPassiveUIUpdate OnUpdatePassiveUI;
};

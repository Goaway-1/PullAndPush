// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData/PassiveItemData/PassiveItemData.h"
#include "CustomPassiveItemData.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UCustomPassiveItemData : public UPassiveItemData
{
	GENERATED_BODY()

public:
	virtual void UsePassiveItem(class AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated) override;
	virtual void EndPassiveItem() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FPassiveStat PassiveStat;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemData/PassiveItemData/PassiveItemData.h"
#include "CustomPassiveItemData.generated.h"


UENUM(BlueprintType)
enum class ECustomStatType : uint8
{
	EST_ItemStat			UMETA(DisplayName = "EST_ItemStat"),
	EST_CharacterStat		UMETA(DisplayName = "EST_CharacterStat")
};

UCLASS()
class PULLANDPUSH_API UCustomPassiveItemData : public UPassiveItemData
{
	GENERATED_BODY()

public:
	virtual void UsePassiveItem(class AActor* TargetActor, FTimerHandle Handler, bool InPassiveItemAlreadyActivated) override;
	virtual void EndPassiveItem() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Item)
	ECustomStatType StatType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (EditCondition = "StatType == ECustomStatType::EST_ItemStat"))
	FItemEnhancedStat PassiveStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (EditCondition = "StatType == ECustomStatType::EST_CharacterStat"))
	ECharacterStat CharacterStat;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PULLANDPUSH_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetInitItem(FTimerHandle TimerHandler, UMaterialInterface* Material, float Cost);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Timer;
	
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ItemMaterial;

private:
	float DefaultItemCost;

	FTimerHandle ItemTimerHandler;
};

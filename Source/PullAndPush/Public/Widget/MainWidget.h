// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateItemUI(class UDataAsset* CurrentItem, const bool& IsPassvieItem);

protected:
	virtual void NativeConstruct() override;

	void UpdatePassiveItemUI(UDataAsset* CurrentItem);
	void UpdateActiveItemUI(UDataAsset* CurrentItem);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> ItemCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ItemHorizontalBox;

private:
	/** Passive Item Widget */
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class UPassiveItemWidget> PassiveItemWidgetClass;

	// Key : FPrimartAssetId, Value : PassiveItemWidget
	UPROPERTY()
	TMap<FName, TWeakObjectPtr<class UPassiveItemWidget>> PassiveItemWidgetMap;

	/** Active Item Widget */
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class UActiveItemWidget> ActiveItemWidgetClass;

	UPROPERTY()
	TObjectPtr<class UActiveItemWidget> ActiveItemWidget;
};

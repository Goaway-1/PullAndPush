// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangeVisibleItemWidget, bool)

UCLASS()
class PULLANDPUSH_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateItemUI(class UDataAsset* CurrentItem, const bool& IsPassvieItem);

protected:
	virtual void NativeConstruct() override;

	void UpdatePassiveItemUI(UDataAsset* CurrentItem);
	void UpdateActiveItemUI(UDataAsset* CurrentItem);

public:
	void UpdateStatUI(const FString& StatName, UMaterialInterface* Material);

protected:
	// Horizontal Box By Widget Type
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> PassiveItemWidgetHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ActiveItemWidgetHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> StatWidgetHorizontalBox;

private:
/** Passive Item Widget */
	UPROPERTY(EditAnywhere, Category = "Widget | Item")
	TSubclassOf<class UPassiveItemWidget> PassiveItemWidgetClass;

	// Key : FPrimartAssetId, Value : PassiveItemWidget
	UPROPERTY()
	TMap<FName, TWeakObjectPtr<class UPassiveItemWidget>> PassiveItemWidgetMap;

/** Active Item Widget */
	UPROPERTY(EditAnywhere, Category = "Widget | Item")
	TSubclassOf<class UActiveItemWidget> ActiveItemWidgetClass;

	UPROPERTY()
	TObjectPtr<class UActiveItemWidget> ActiveItemWidget;

/** Character Stat Widget */
	UPROPERTY(EditAnywhere, Category = "Widget | Stat")
	TSubclassOf<class UStatWidget> StatWidgetClass;

	UPROPERTY()
	TMap<FString, TWeakObjectPtr<class UStatWidget>> StatWidgetMap;

public:
	// Change Active Item Visible
	FOnChangeVisibleItemWidget OnChangeVisibleItemWidget;
};

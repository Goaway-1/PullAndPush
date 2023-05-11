// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActiveItemWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PULLANDPUSH_API UActiveItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/** Update Active item 	*/
	void UpdateItem(class UDataAsset* ItemData= nullptr);

	/** Set Visibility Active item info	*/
	UFUNCTION()
	void ChangeVisibleItemInfo(bool bVisibility);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> TextCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DescriptionText;

// Data
protected:
	UPROPERTY(EditDefaultsOnly, Category="Item | Material")
	TObjectPtr<class UMaterialInterface> DefaultMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TWeakObjectPtr<class UDataAsset> CurrentItemData;
};

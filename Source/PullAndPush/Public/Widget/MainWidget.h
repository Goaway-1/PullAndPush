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
	UMainWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateItemUI(class UDataAsset* CurrentItem);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> ItemCanvasPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ItemHorizontalBox;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<class UItemWidget> ItemWidgetClass;

private:
	TMap<FName, TObjectPtr<class UItemWidget>> ItemMap;
};

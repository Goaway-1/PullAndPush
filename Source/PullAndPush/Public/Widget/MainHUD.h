// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UMainWidget> MainWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMainWidget> MainWidget;

/** Funtion */
public:
	void UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem);

	void ChangeVisibleItemInfo(bool bVisibility);

	void UpdateStatUI(const FString& StatName, UMaterialInterface* Material);
};

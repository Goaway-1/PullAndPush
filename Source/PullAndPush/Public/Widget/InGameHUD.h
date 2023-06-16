// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UInGameWidget> InGameWidgetClass;

	UPROPERTY()
	TObjectPtr<class UInGameWidget> InGameWidget;

/** Funtion */
public:
	void UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem);

	void ChangeVisibleItemInfo(bool bVisibility);

	void UpdateStatUI(const FString& StatName, UMaterialInterface* Material);
};

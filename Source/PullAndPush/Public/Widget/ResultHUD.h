// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ResultHUD.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API AResultHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

public:
	void ShowResult(TMap<FString, int8>& ScoreInfo);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UResultWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<class UResultWidget> ResultWidget;
};

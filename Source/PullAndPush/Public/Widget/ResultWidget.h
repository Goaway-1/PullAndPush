// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetScoreBoard(TMap<FString, int8>& ScoreInfo);

private:
	UFUNCTION()
	void OnGotoMainClicked();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget | Class")
	TSubclassOf<class UScoreBoardWidget> ScoreBoardWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> ScoreBoardBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MainButton;
};

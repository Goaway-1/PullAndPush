// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBoardWidget.generated.h"

/**
 * Show Player Score...
 */
UCLASS()
class PULLANDPUSH_API UScoreBoardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPlayerScoreBoard(int8 Rank, const FString& Name, int8 Score);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerRank;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerScore;
};

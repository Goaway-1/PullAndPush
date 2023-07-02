// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpectatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API USpectatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitPlayerCount(int8 InTotalPlayerCount);
	void SetCurrentPlayerCount(int8 InCount);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TotalCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentCount;
};

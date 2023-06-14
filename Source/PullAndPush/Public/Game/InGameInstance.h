// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InGameInstance.generated.h"

/**
 * Store information that must be maintained when levels change
 */
UCLASS()
class PULLANDPUSH_API UInGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UInGameInstance();

	/** Whether the first round has begun */
	bool IsFirstRoundStart();

	void InitSetting(int InMaxRoundCount);

	/** Whether the all rounds are over */
	bool IsAllRoundsFinished();

protected:
	/** Round Info */
	uint8 bIsFirstRoundStart : 1;
	int MaxRoundCount;
	int CurrentRoundCount;
};

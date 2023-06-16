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

/** Round Info */
#pragma region ROUND
public:
	/** Whether the first round has begun */
	bool IsFirstRoundStart();

	void InitSetting(const int8 InMaxRoundCount);

	/** Whether the all rounds are over */
	bool IsAllRoundsFinished();

protected:
	/** Round Info */
	uint8 bIsFirstRoundStart : 1;
	int8 MaxRoundCount;
	int8 CurrentRoundCount;
#pragma endregion

/** Player Score */
#pragma region PLAYERSCORE
public:
	void SetPlayersScore(TMap<FString, int8>& Controllers);

protected:
	/**
	* Save Controller's Score
	* @param	FString			Controller Name
	* @param	int8			Score
	*/
	UPROPERTY()
	TMap<FString, int8> PlayersScore;

#pragma endregion
		
};

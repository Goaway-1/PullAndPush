// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/GameModeBase.h"
#include "InGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API AInGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInGameMode();

	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	/** Decrease player count if character is fell out of world */
	void PlayerFellOutOfWorld();

private:
	/** Start round if all players are logged in to the level */
	void RoundStart();

	/** Finsh round if only one player is survive */
	void RoundEnd();

	/** Move to the next round */
	void SwitchToNextLevel();

	/** Get Level Path In FString */
	FString GetRandomLevelName();

	/** If all rounds have been completed, move to the result level */
	void AllRoundsCompleted();

private:
	// Player Count
	UPROPERTY(VisibleAnywhere, Category = "Data")
	int TotalPlayerCount;								// @TODO : 로비에서 파악 후 넘겨주어야 함.

	UPROPERTY(VisibleAnywhere, Category = "Data")
	int CurrentPlayerCount;

	// Round Count used in 'GameInstance'
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int MaxRoundCount;

	UPROPERTY(VisibleAnywhere, Category = "Level")
	TObjectPtr<class UInGameInstance> InGameInstance;

	// To Get Random Level 
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int MaxLevelCount;

	const FString LevelDirectoryPath = "/Game/Maps/Level";
	const FString ResultLevelName = "/Game/Maps/ResultLevel";
	const FString LobbyLevelName = "/Game/Maps/LobbyLevel";
};

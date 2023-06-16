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
	void PlayerFellOutOfWorld(const FString& ControllerName);

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
	int8 TotalPlayerCount;								// @TODO : 로비에서 파악 후 넘겨주어야 함.

	UPROPERTY(VisibleAnywhere, Category = "Data")
	int8 CurrentPlayerCount;

	// Round Count used in 'GameInstance'
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int8 MaxRoundCount;

	UPROPERTY(VisibleAnywhere, Category = "Level")
	TObjectPtr<class UInGameInstance> InGameInstance;

	// Get Random Level 
	UPROPERTY(EditDefaultsOnly, Category = "Level")
	int8 MaxLevelCount;

	const FString LevelDirectoryPath = "/Game/Maps/Level";
	const FString ResultLevelName = "/Game/Maps/ResultLevel";
	const FString LobbyLevelName = "/Game/Maps/LobbyLevel";

#pragma region SCORE
private:
	/** Initialize the score to be given to the player's controller */
	void InitPlayersScore(APlayerController* NewPlayer);

	void SetPlayerScore(const FString & ControllerName);

	/** Give score to the characters who survive to the end */
	void CalculatePlayerScore();

private:
	/**
	* Save Controller's Score
	* @param	FString			Controller Name
	* @param	int8			Score
	*/
	UPROPERTY()
	TMap<FString, int8> PlayersScore;

	int8 CurrentScore;
	const int8 InitialScore = -1;
#pragma endregion
};

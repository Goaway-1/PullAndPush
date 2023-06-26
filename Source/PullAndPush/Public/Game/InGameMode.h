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

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
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

	/** If all rounds have been completed, move to the result level */
	void AllRoundsCompleted();

private:
	// Player Count
	int8 TotalPlayerCount;
	int8 CurrentPlayerCount;

	//@@TODO : ¼öÁ¤
	// Round Count used in 'GameInstance'
	//UPROPERTY(EditDefaultsOnly, Category = "Data")
	//int8 MaxRoundCount;

	UPROPERTY()
	TObjectPtr<class UInGameInstance> InGameInstance;

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
	TMap<FString, int8> Controllers;

	int8 CurrentScore;
	const int8 InitialScore = -1;
#pragma endregion
};

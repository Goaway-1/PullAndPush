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
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

#pragma region ROUND
public:
	/** Decrease player count if character is fell out of world */
	UFUNCTION(BlueprintNativeEvent)
	void PlayerFellOutOfWorld(const FString& InPlayerName);

private:
	void ReadyToRoundStart(APlayerController* NewPlayer);

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
	const float RoundStartTime = 3.f;

	UPROPERTY()
	TObjectPtr<class UInGameInstance> InGameInstance;
#pragma endregion

#pragma region SCORE
public:
	FORCEINLINE int8 GetCurrentScore() { return CurrentScore++; }
private:
	void InitPlayers(APlayerController* NewPlayer);

private:
	UPROPERTY()
	TArray<class APlayableController*> Controllers;

	int8 CurrentScore;
#pragma endregion
};

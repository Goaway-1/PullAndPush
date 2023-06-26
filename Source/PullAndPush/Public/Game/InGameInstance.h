// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InGameInstance.generated.h"

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	None	= 0	UMETA(Hidden),
	ELT_Main	UMETA(DisplayName = "Main"),
	ELT_Lobby   UMETA(DisplayName = "Lobby"),
	ELT_InGame	UMETA(DisplayName = "InGame"),
	ELT_Result	UMETA(DisplayName = "Result")
};

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
	/** Whether the all rounds are over */
	bool IsAllRoundsFinished();

private:
	void ResetData();

private:
	/** Round Info */
	UPROPERTY(EditDefaultsOnly, Category = "Game | Round")
	int8 MaxRoundCount;

	int8 CurrentRoundCount;
#pragma endregion

/** Player Count & Score */
#pragma region PLAYER
public:
	// Count
	void InitTotalPlayerCount();
	void AddTotalPlayerCount();
	FORCEINLINE int8 GetTotalPlayerCount() { return TotalPlayerCount; }

	// Score
	void SetPlayersScore(TMap<FString, int8>& Controllers);
	TMap<FString, int8>& GetPlayersScore();

private:
	int8 TotalPlayerCount;

	/**
	* Save Controller's Score
	* @param	FString			Controller Name
	* @param	int8			Score
	*/
	UPROPERTY()
	TMap<FString, int8> PlayersScore;

#pragma endregion

#pragma region LEVEL
public:
	void TravelLevel(ELevelType LevelType);

private:
	/** Randomly gets names among InGame's levels */
	FString GetRandomLevelName();

	/** Get correct level path of an enum */
	FString GetLevelPathOfEnum(ELevelType LevelType);
private:
	const FString LevelDirectoryPath = "/Game/Maps/InGame/";
	FName MainLevelName = "MainLevel";
	const FString LobbyLevelName = "/Game/Maps/LobbyLevel";
	const FString ResultLevelName = "/Game/Maps/ResultLevel";
#pragma endregion

};

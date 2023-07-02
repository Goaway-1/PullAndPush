#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Kismet/GameplayStatics.h"

AInGameMode::AInGameMode()
	:
	TotalPlayerCount(0), CurrentPlayerCount(0), CurrentScore(1)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
}
void AInGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Set total player count
	InGameInstance = Cast<UInGameInstance>(GetGameInstance());
	if (InGameInstance)
	{
		TotalPlayerCount = InGameInstance->GetTotalPlayerCount();
	}
}
void AInGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	InitPlayersScore(NewPlayer);

	/** Round start when all players enter */
	if (++CurrentPlayerCount >= TotalPlayerCount)
	{
		RoundStart();
	}
}
void AInGameMode::PlayerFellOutOfWorld(const FString& ControllerName)
{
	SetPlayerScore(ControllerName);

	// Check Round Is End?
	if (--CurrentPlayerCount <= 1)
	{
		RoundEnd();
	}
}
void AInGameMode::RoundStart()
{
	PPLOG(Warning, TEXT("RoundStart!"));
}
void AInGameMode::RoundEnd()
{
	CalculatePlayerScore();

	// Clear all timer of characters..
	for (auto Controller : Controllers)
	{
		Controller->ClearAllTimer();
	}

	// Access GameInstance and Switch Level
	if(!InGameInstance->IsAllRoundsFinished())
	{
		SwitchToNextLevel();
	}
	else
	{
		AllRoundsCompleted();
	}
}
void AInGameMode::SwitchToNextLevel()
{
	InGameInstance->TravelLevel(ELevelType::ELT_InGame);
}
void AInGameMode::AllRoundsCompleted()
{
	InGameInstance->TravelLevel(ELevelType::ELT_Result);
}
void AInGameMode::InitPlayersScore(APlayerController* NewPlayer)
{
	Controllers.Add(NewPlayer->GetName(), InitialScore);
}
void AInGameMode::SetPlayerScore(const FString& ControllerName)
{
	Controllers[ControllerName] = CurrentScore++;
}
void AInGameMode::CalculatePlayerScore()
{
	// Give score to the characters who survive to the end
	for (auto& Controller : Controllers)
	{
		if (Controller.Value == InitialScore)
		{
			Controller.Value = CurrentScore;
			break;
		}
	}

	// Access GameInstance and set player score
	if (InGameInstance)
	{
		InGameInstance->SetPlayersScore(Controllers);
	}
}
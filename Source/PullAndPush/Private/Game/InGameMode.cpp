#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayableController.h"

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

	InitPlayers(NewPlayer);

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

	// Set Current Player Count for Widget
	for (auto Controller : Controllers)
	{
		Controller->SetCurrentPlayerCount(CurrentPlayerCount);
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
		Controller->UnPossess();
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
void AInGameMode::InitPlayersScore(const FString& ControllerName)
{
	ControllersScore.Add(ControllerName, InitialScore);
}
void AInGameMode::InitPlayers(APlayerController* NewPlayer)
{
	APlayableController* PlayableController = Cast<APlayableController>(NewPlayer);
	if (PlayableController)
	{
		Controllers.Add(PlayableController);
		PlayableController->InitPlayerCount(TotalPlayerCount);
	}
}
void AInGameMode::SetPlayerScore(const FString& ControllerName)
{
	if (ControllersScore.Num() > 0)
	{
		ControllersScore[ControllerName] = CurrentScore++;
	}
}
void AInGameMode::CalculatePlayerScore()
{
	// Give score to the characters who survive to the end
	for (auto& Controller : ControllersScore)
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
		InGameInstance->SetPlayersScore(ControllersScore);
	}
}
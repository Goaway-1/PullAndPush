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
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AInGameMode::RoundStart, 1.5f);
	}
}
void AInGameMode::PlayerFellOutOfWorld_Implementation(const FString& InPlayerName)
{
	SetPlayerScore(InPlayerName);

	// Check Round Is End?
	if (--CurrentPlayerCount <= 1)
	{
		RoundEnd();
	}

	// Set Current Player Count for Widget
	for (auto Controller : Controllers)
	{
		Controller->ClientSetCurrentPlayerCount(CurrentPlayerCount);
	}
}
void AInGameMode::RoundStart()
{
	// Change Widget of Round State
	for (auto Controller : Controllers)
	{
		Controller->ClientSetRoundStart();
	}
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
void AInGameMode::InitPlayersScore(const FString& InPlayerName)
{
	ControllersScore.Add(InPlayerName, InitialScore);
}
void AInGameMode::InitPlayers(APlayerController* NewPlayer)
{
	APlayableController* PlayableController = Cast<APlayableController>(NewPlayer);
	if (PlayableController)
	{
		Controllers.Add(PlayableController);
		PlayableController->ClientInitPlayerCount(TotalPlayerCount);
	}
}
void AInGameMode::SetPlayerScore(const FString& InPlayerName)
{
	if (ControllersScore.Num() > 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s 's Score : %d"), *InPlayerName, CurrentScore);
		ControllersScore[InPlayerName] = CurrentScore++;
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
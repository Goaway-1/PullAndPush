#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayableController.h"
#include "GameFramework/GameState.h"
#include "Player/PlayableState.h"

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

	ReadyToRoundStart(NewPlayer);
}
void AInGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);

	ReadyToRoundStart(NewPC);
}
void AInGameMode::PlayerFellOutOfWorld_Implementation(const FString& InPlayerName)
{
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
void AInGameMode::ReadyToRoundStart(APlayerController* NewPlayer)
{
	InitPlayers(NewPlayer);

	/** Round start when all players enter */
	if (++CurrentPlayerCount >= TotalPlayerCount)
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AInGameMode::RoundStart, 1.5f);
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
	// Find the last standing player and give score
	for (auto PlayerState : GameState->PlayerArray)
	{
		if (APlayableState* NewPlayerState = Cast<APlayableState>(PlayerState))
		{
			if (!NewPlayerState->GetIsDead())
			{
				int8 NewScore = GetCurrentScore() + NewPlayerState->GetRankScore();
				NewPlayerState->SetRankScore(NewScore);
				break;
			}
		}
	}

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
void AInGameMode::InitPlayers(APlayerController* NewPlayer)
{
	APlayableController* PlayableController = Cast<APlayableController>(NewPlayer);
	if (PlayableController)
	{
		Controllers.Add(PlayableController);
		PlayableController->ClientInitPlayerCount(TotalPlayerCount);
	}
}
#include "Game/InGameMode.h"
#include "Game/InGameInstance.h"
#include "Kismet/GameplayStatics.h"

AInGameMode::AInGameMode()
	:
	TotalPlayerCount(0), CurrentPlayerCount(0), MaxRoundCount(2), MaxLevelCount(0), CurrentScore(1)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
}
void AInGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Set GameInstance Value
	InGameInstance = Cast<UInGameInstance>(GetGameInstance());
	if (InGameInstance)
	{
		if (!InGameInstance->IsFirstRoundStart())
		{
			InGameInstance->InitSetting(MaxRoundCount);
		}

		// Set total player count
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
	FString NextLevelName = GetRandomLevelName();
	GetWorld()->ServerTravel(NextLevelName);
}
void AInGameMode::AllRoundsCompleted()
{
	GetWorld()->ServerTravel(ResultLevelName);
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
FString AInGameMode::GetRandomLevelName()
{
	int LevelIdx = FMath::RandRange(1, MaxLevelCount);
	FString LevelName = LevelDirectoryPath + FString::FromInt(LevelIdx);
	return LevelName;
}
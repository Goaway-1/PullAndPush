#include "Game/ResultGameMode.h"
#include "Game/InGameInstance.h"
#include "Player/UIController.h"
#include "Player/PlayableState.h"
#include "GameFramework/GameState.h"

AResultGameMode::AResultGameMode()
	:
	TotalPlayerCount(0), CurrentPlayerCount(0)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
}
void AResultGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Set GameInstance Value
	InGameInstance = Cast<UInGameInstance>(GetGameInstance());
	if (InGameInstance)
	{
		TotalPlayerCount = InGameInstance->GetTotalPlayerCount();
	}
}
void AResultGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	InitSetting(NewPlayer);
}
void AResultGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	
	InitSetting(NewPC);

	// @TODO : 기존 HUD 삭제
}
void AResultGameMode::InitSetting(APlayerController* NewPlayer)
{
	Controllers.Add(NewPlayer);

	// Ready To Start
	if (++CurrentPlayerCount >= TotalPlayerCount)
	{
		FTimerHandle Handler;
		GetWorld()->GetTimerManager().SetTimer(Handler, this, &AResultGameMode::ShowScoreBoard, 1.f, false);
	}
}
void AResultGameMode::ShowScoreBoard()
{	
	// Change rank-scores to TMap
	TMap<FString, int8> RankScore;
	for (auto PlayerState : GameState->PlayerArray)
	{
		if (APlayableState* NewPlayerState = Cast<APlayableState>(PlayerState))
		{
			RankScore.Emplace(NewPlayerState->GetPlayerName(), NewPlayerState->GetRankScore());
		}
	}

	// Show Result Score
	for (auto Controller : Controllers)
	{
		AUIController* PlayerUIController = Cast<AUIController>(Controller);
		if (PlayerUIController)
		{
			PlayerUIController->ShowResult(RankScore);
		}
	}
}
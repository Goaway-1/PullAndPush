#include "Game/ResultGameMode.h"
#include "Game/InGameInstance.h"
#include "Player/UIController.h"

AResultGameMode::AResultGameMode()
	:
	TotalPlayerCount(3), CurrentPlayerCount(0)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
}
void AResultGameMode::StartPlay()
{
	Super::StartPlay();

	// Set GameInstance Value
	InGameInstance = Cast<UInGameInstance>(GetGameInstance());
}
void AResultGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	Controllers.Add(NewPlayer);

	/** Round start when all players enter */
	if (++CurrentPlayerCount >= TotalPlayerCount)
	{
		FTimerHandle Handler;
		GetWorld()->GetTimerManager().SetTimer(Handler, this, &AResultGameMode::ShowScoreBoard, 1.f, false);
	}
}
void AResultGameMode::ShowScoreBoard()
{	
	if (!InGameInstance) return;

	for (auto Controller : Controllers)
	{
		AUIController* PlayerUIController = Cast<AUIController>(Controller);
		if (PlayerUIController)
		{
			PlayerUIController->ShowResult(InGameInstance->GetPlayersScore());
		}
	}
}
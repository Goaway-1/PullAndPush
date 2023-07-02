#include "Game/LobbyGameMode.h"
#include "Player/UIController.h"
#include "Game/InGameInstance.h"

void ALobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	// Reset total player count
	InGameInstance = Cast<UInGameInstance>(GetGameInstance());
	if (InGameInstance)
	{
		InGameInstance->InitTotalPlayerCount();
	}
}
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Add total player count
	if (InGameInstance)
	{
		InGameInstance->AddTotalPlayerCount();
	}

	SetLobbyWidgetData(NewPlayer);
}
void ALobbyGameMode::SetLobbyWidgetData(APlayerController* NewPlayer)
{
	// Add Controllers
	if (!InGameInstance) return;
	AUIController* NewUIController = Cast<AUIController>(NewPlayer);
	Controllers.Add(NewUIController);

	// Try to set Lobby Data to widget
	for (auto Controller : Controllers)
	{
		Controller->SetLobbyWidgetData(InGameInstance->GetMaxPlayerCount(), InGameInstance->GetTotalPlayerCount());
	}
}
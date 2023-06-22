#include "Game/LobbyGameMode.h"
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
}
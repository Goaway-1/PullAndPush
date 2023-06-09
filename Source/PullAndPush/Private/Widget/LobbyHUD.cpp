#include "Widget/LobbyHUD.h"
#include "Widget/LobbyWidget.h"
#include "Game/InGameInstance.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();

	ensure(LobbyWidgetClass);
	const auto PlayerController = GetOwningPlayerController();
	if (PlayerController && PlayerController->HasAuthority())
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(PlayerController, LobbyWidgetClass);
		LobbyWidget->AddToViewport();
	}
}
void ALobbyHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ALobbyHUD::SetLobbyWidgetData(int8 InMaxPlayerCount, int8 InTotalPlayerCount)
{
	LobbyWidget->SetLobbyWidgetData(InMaxPlayerCount, InTotalPlayerCount);
}

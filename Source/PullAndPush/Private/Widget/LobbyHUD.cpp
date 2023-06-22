#include "Widget/LobbyHUD.h"
#include "Widget/LobbyWidget.h"

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

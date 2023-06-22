#include "Widget/LobbyWidget.h"
#include "Components/Button.h"
#include "Game/InGameInstance.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::StartGame);
}
void ULobbyWidget::StartGame()
{
    UInGameInstance* GameInstance = Cast<UInGameInstance>(GetGameInstance());
    GameInstance->TravelLevel(ELevelType::ELT_InGame);
}
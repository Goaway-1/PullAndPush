#include "Widget/LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/InGameInstance.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::StartGame);
}
void ULobbyWidget::StartGame()
{
    UInGameInstance* GameInstance = Cast<UInGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->TravelLevel(ELevelType::ELT_InGame);
    }
}
void ULobbyWidget::SetLobbyWidgetData(int8 InMaxPlayerCount, int8 InTotalPlayerCount)
{
    TotalPlayerCount->SetText(FText::AsNumber(InTotalPlayerCount));
    MaxPlayerCount->SetText(FText::AsNumber(InMaxPlayerCount));
}

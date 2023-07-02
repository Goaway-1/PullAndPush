#include "Widget/InGameHUD.h"
#include "Widget/InGameWidget.h"
#include "Widget/SpectatorWidget.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	const auto PlayerController = GetOwningPlayerController();
	if (PlayerController && InGameWidgetClass && SpectatorWidgetClass)
	{
		InGameWidget = CreateWidget<UInGameWidget>(PlayerController, InGameWidgetClass);
		SpectatorWidget = CreateWidget<USpectatorWidget>(PlayerController, SpectatorWidgetClass);

		InGameWidget->AddToViewport();
		SpectatorWidget->AddToViewport();

		SpectatorWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}
void AInGameHUD::UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem)
{
	InGameWidget->UpdateItemUI(CurrentItem, IsPassvieItem);
}
void AInGameHUD::ChangeVisibleItemInfo(bool bVisibility)
{
	InGameWidget->OnChangeVisibleItemWidget.Execute(bVisibility);
}
void AInGameHUD::UpdateStatUI(const FString& StatName, UMaterialInterface* Material)
{
	InGameWidget->UpdateStatUI(StatName, Material);
<<<<<<< Updated upstream
}
=======
}
void AInGameHUD::InitPlayerCount(int8 InTotalPlayerCount)
{
	InGameWidget->InitPlayerCount(InTotalPlayerCount);
	SpectatorWidget->InitPlayerCount(InTotalPlayerCount);
}
void AInGameHUD::SetCurrentPlayerCount(int8 InCount)
{
	InGameWidget->SetCurrentPlayerCount(InCount);
	SpectatorWidget->SetCurrentPlayerCount(InCount);
}
EHUDState AInGameHUD::GetCurrentState() const
{
	return CurrentState;
}
void AInGameHUD::OnStateChanged(EHUDState NewState)
{
	CurrentState = NewState;

	// Visible SpectatorWidget..
	if (CurrentState == EHUDState::Spectating)
	{
		InGameWidget->SetVisibility(ESlateVisibility::Hidden);
		SpectatorWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
>>>>>>> Stashed changes

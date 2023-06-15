#include "Widget/InGameHUD.h"
#include "Widget/InGameWidget.h"

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	const auto PlayerController = GetOwningPlayerController();
	if (PlayerController && InGameWidgetClass)
	{
		InGameWidget = CreateWidget<UInGameWidget>(PlayerController, InGameWidgetClass);
		InGameWidget->AddToViewport();
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
}
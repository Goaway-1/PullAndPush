#include "Widget/MainHUD.h"
#include "Widget/MainWidget.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	const auto PlayerController = GetOwningPlayerController();
	if (PlayerController && MainWidgetClass)
	{
		MainWidget = CreateWidget<UMainWidget>(PlayerController, MainWidgetClass);
		MainWidget->AddToViewport();
	}
}

void AMainHUD::UpdateItemUI(UDataAsset* CurrentItem, const bool IsPassvieItem)
{
	MainWidget->UpdateItemUI(CurrentItem, IsPassvieItem);
}
void AMainHUD::ChangeVisibleItemInfo(bool bVisibility)
{
	MainWidget->OnChangeVisibleItemWidget.Execute(bVisibility);
}
void AMainHUD::DrawHUD()
{
	Super::DrawHUD();
}
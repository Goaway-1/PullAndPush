#include "Widget/ResultHUD.h"
#include "Widget/ResultWidget.h"

void AResultHUD::BeginPlay()
{
	Super::BeginPlay();

	const auto PlayerController = GetOwningPlayerController();
	if (PlayerController && ResultWidgetClass)
	{
		ResultWidget = CreateWidget<UResultWidget>(PlayerController, ResultWidgetClass);
		ResultWidget->AddToViewport();
	}
}
void AResultHUD::DrawHUD()
{
	Super::DrawHUD();
}
void AResultHUD::ShowResult(TMap<FString, int8>& ScoreInfo)
{
	ResultWidget->SetScoreBoard(ScoreInfo);
}

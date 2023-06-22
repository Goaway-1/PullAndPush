#include "Widget/ResultWidget.h"
#include "Widget/ScoreBoardWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ensure(ScoreBoardWidgetClass);
    MainButton->OnClicked.AddDynamic(this, &UResultWidget::OnGotoMainClicked);
}
void UResultWidget::SetScoreBoard(TMap<FString, int8>& ScoreInfo)
{
    int8 idx = 0;
    for (auto& Score : ScoreInfo)
    {
        /** Create Active Widget */
        UScoreBoardWidget* ScoreBoardWidget = CreateWidget<UScoreBoardWidget>(this, ScoreBoardWidgetClass);

        if (ScoreBoardWidget)
        {
            ScoreBoardWidget->SetPlayerScoreBoard(++idx, Score.Key, Score.Value);
            ScoreBoardBox->AddChildToVerticalBox(ScoreBoardWidget);
        }
    }
}
void UResultWidget::OnGotoMainClicked()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        UGameplayStatics::OpenLevel(this, "MainLevel");
    }
}
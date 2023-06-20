#include "Widget/ScoreBoardWidget.h"
#include "Components/TextBlock.h"

void UScoreBoardWidget::SetPlayerScoreBoard(int8 Rank, const FString& Name, int8 Score)
{
	PlayerRank->SetText(FText::AsNumber(Rank));
	PlayerName->SetText(FText::FromString(Name));
	PlayerScore->SetText(FText::AsNumber(Score));
}
#include "Player/UIController.h"
#include "Widget/ResultHUD.h"
#include "Widget/LobbyHUD.h"

AUIController::AUIController() 
	:
	bIsResultController(0)
{
	bShowMouseCursor = true;			
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}
void AUIController::BeginPlay()
{
	Super::BeginPlay();

	if(bIsResultController)
	{
		ResultHUD = Cast<AResultHUD>(GetHUD());
	}
	else
	{
		LobbyHUD = Cast<ALobbyHUD>(GetHUD());
	}
}
void AUIController::SetLobbyWidgetData(int8 InMaxPlayerCount, int8 InTotalPlayerCount)
{
	if (LobbyHUD.IsValid() && LobbyHUD.Get()->SetLobbyWidgetData(InMaxPlayerCount, InTotalPlayerCount))
	{
		PPLOG(Log, TEXT("InitPlayer Count Successed!"));
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &AUIController::SetLobbyWidgetData, InMaxPlayerCount, InTotalPlayerCount));
	}
}
void AUIController::ShowResult(TMap<FString, int8>& PlayersScore)
{
	// Sort
	PlayersScore.ValueSort([](int8 A, int8 B) {
		return A > B; 
		});

	// TMap to TArray
	TArray<FString> Keys;
	TArray<int8> Values;
	for (const auto& Pair : PlayersScore)
	{
		Keys.Add(Pair.Key);
		Values.Add(Pair.Value);
	}

	ClientShowResult(Keys, Values);
}
void AUIController::ClientShowResult_Implementation(const TArray<FString>& KeyArray, const TArray<int8>& ValueArray)
{
	if (!ResultHUD.Get()) return;
	
	// TArray to TMap
	TMap<FString, int8> ScoreInfo;
	for (int8 Index = 0; Index < KeyArray.Num(); Index++)
	{
		ScoreInfo.Add(KeyArray[Index], ValueArray[Index]);
	}

	// Show Result Score Board
	ResultHUD.Get()->ShowResult(ScoreInfo);
}

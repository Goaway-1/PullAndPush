#include "Player/UIController.h"
#include "Widget/ResultHUD.h"
#include "Kismet/GameplayStatics.h"	

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
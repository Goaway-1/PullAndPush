#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ResultHUD.generated.h"

/**
 * Result level HUD for player
 */
UCLASS()
class PULLANDPUSH_API AResultHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// Returns true if widget is created, and saves data for widget
	void ShowResult(TMap<FString, int8>& ScoreInfo);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UResultWidget> ResultWidgetClass;

	UPROPERTY()
	TObjectPtr<class UResultWidget> ResultWidget;
};

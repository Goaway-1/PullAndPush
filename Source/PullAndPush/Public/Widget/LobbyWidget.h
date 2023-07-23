#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
* Host can show this widget
*/
UCLASS()
class PULLANDPUSH_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void StartGame();

	void SetLobbyWidgetData(int8 InMaxPlayerCount, int8 InTotalPlayerCount);
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxPlayerCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TotalPlayerCount;
};

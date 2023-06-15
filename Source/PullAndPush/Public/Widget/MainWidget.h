#pragma once

#include "PullAndPush.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

private:
	// Button
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> HostButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> JoinButton;

	// Text
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HostText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> JoinText;

	const FName LobbyLevelName = "/Game/Maps/LobbyLevel";
};

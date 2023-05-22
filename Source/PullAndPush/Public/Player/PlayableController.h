// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayableController.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API APlayableController : public APlayerController
{
	GENERATED_BODY()
public:
	APlayableController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Client, Reliable)
	void UpdateItemUI(class UDataAsset* CurrentItem, const bool IsPassvieItem);

	UFUNCTION(Client, Reliable)
	void ChangeVisibleItemInfo(bool bVisibility);
private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UMainWidget> MainWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMainWidget> MainWidget;
};

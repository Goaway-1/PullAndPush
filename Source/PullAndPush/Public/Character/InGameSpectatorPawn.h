// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "InGameSpectatorPawn.generated.h"

UCLASS()
class PULLANDPUSH_API AInGameSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputAction> SpectateAction;

	void InitEnhancedInput();

private:
	UFUNCTION()
	void SpectateAnotherPlayer();
};

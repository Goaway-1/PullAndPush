// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStatWidget(UMaterialInterface* Material);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> StatIcon;

private:
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> StatMaterial;
};

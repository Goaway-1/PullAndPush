// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/ItemAssetManager.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemActionType : uint8 {
	EIAT_Default = 0		UMETA(DisplayName = "Default"),
	EIAT_P_SpeedUp 		UMETA(DisplayName = "P_SpeedUp"),			// P는 패시브
	EIAT_P_PowerUp 		UMETA(DisplayName = "P_PowerUp"),			// P는 패시브
	EIAT_A_Bomb			UMETA(DisplayName = "A_Bomb")				// A는 액티브
};

UCLASS()
class PULLANDPUSH_API UItem : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	class UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	EItemActionType ItemActionType;		
	
	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 {
	EIT_Default = 0		UMETA(DisplayName = "Default"),
	EIT_Active			UMETA(DisplayName = "Active"),
	EIT_Passive			UMETA(DisplayName = "Passive")
};

UENUM(BlueprintType)
enum class EItemActionType : uint8 {
	EIAT_Default = 0		UMETA(DisplayName = "Default"),
	EIAT_P_SpeedUp 		UMETA(DisplayName = "P_SpeedUp"),			// P는 패시브
	EIAT_P_PowerUp 		UMETA(DisplayName = "P_PowerUp"),			// P는 패시브
	EIAT_A_Bomb			UMETA(DisplayName = "A_Bomb")				// A는 액티브
};

/*
* Active Item : "InterfaceUI, ProjectileItem" must be required
* Passive Item : "DurationTime, WeightValue" must be required
*/
UCLASS()
class PULLANDPUSH_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString Description;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere)
	EItemType ItemType;					

	UPROPERTY(EditAnywhere)
	EItemActionType ItemActionType;				

	/** For Active Item */
	UPROPERTY(EditAnywhere)
	class UTexture2D* InterfaceUI;	

	// @TODO : 투사체 Item으로 변경
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> ProjectileItemClass;

	/** For Passive Item */
	UPROPERTY(EditAnywhere)
	float DurationTime;				

	UPROPERTY(EditAnywhere)
	float WeightValue;
};

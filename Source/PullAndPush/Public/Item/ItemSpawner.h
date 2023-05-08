// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Item/ItemPickup.h"
#include "Item/ItemData/ItemData.h"
#include "ItemSpawner.generated.h"

UENUM(BlueprintType)
enum class EItemSpawnType : uint8
{
	Normal = 0,
	Passive,
	Active
};

UCLASS()
class PULLANDPUSH_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemSpawner();

protected:
	virtual void BeginPlay() override;

private:
	void InitSetting();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AItemPickup> ItemPickupClass;

	UPROPERTY()
	TObjectPtr<class AItemPickup> ItemPickup;

	UPROPERTY(EditAnywhere, Category = "Item | Spawn Type")
	EItemSpawnType ItemSpawnType;

	FVector SpawnLocation;

	/** Respawn */
	UFUNCTION()
	void TryRespawnItem();

	UFUNCTION()
	void RespawnItem();

	UPROPERTY(EditAnywhere, Category = "Item | Spawn Time")
	float MaxRespawnDelay;

	UPROPERTY(EditAnywhere, Category="Item | Spawn Time")
	float MinRespawnDelay;

	UPROPERTY()
	FTimerHandle RespawnHandle;

	/** Item Data*/
	UFUNCTION()
	class UItemData* SetRandomItemDataAsset();
};

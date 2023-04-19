// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Item/ItemPickup.h"
#include "Item/Item.h"
#include "ItemSpawner.generated.h"


UCLASS()
class PULLANDPUSH_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void InitSetting();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AItemPickup> ItemPickupClass;

	UPROPERTY()
	TObjectPtr<class AItemPickup> ItemPickup;

	FVector SpawnLocation;

	/** Respawn */
	UFUNCTION()
	void TryRespawnItem();

	UFUNCTION()
	void RespawnItem();

	UPROPERTY(EditAnywhere, Category = "Spawn Time")
	float MaxRespawnDelay;

	UPROPERTY(EditAnywhere, Category="Spawn Time")
	float MinRespawnDelay;

	UPROPERTY()
	FTimerHandle RespawnHandle;

	/** Item Data*/
	UFUNCTION()
	class UItem* SetRandomItemDataAsset();

	UPROPERTY(EditDefaultsOnly, Category="Item")
	TArray<TObjectPtr<class UItem>> ItemDataArray;

};

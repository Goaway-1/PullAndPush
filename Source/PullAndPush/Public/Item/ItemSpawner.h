// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Item/ItemPickup.h"
#include "TimerManager.h"
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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AItemPickup> ItemPickupClass;

private:
	void InitSetting();

	UPROPERTY()
	TObjectPtr<class AItemPickup> ItemPickup;

	FVector SpawnLocation;

	/* Respawn */
	UFUNCTION()
	void TryRespawnItem();

	UFUNCTION()
	void RespawnItem();

	const float MaxRespawnDelay = 10.f;
	const float MinRespawnDelay = 3.f;

	UPROPERTY()
	FTimerHandle RespawnHandle;
};

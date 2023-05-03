// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickup.generated.h"

DECLARE_DELEGATE(FItemPickupOnAction)

UCLASS()
class PULLANDPUSH_API AItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemPickup();

protected:
	virtual void BeginPlay() override;

public:	
	FItemPickupOnAction OnPickupAction;

	// Settings when an item is created or used
	UFUNCTION()
	void SetItemSetting(bool IsSpawn, class UItem* InItemDataAsset = nullptr, FVector SpawnLocation = FVector(0.f));
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category="Item")
	TObjectPtr<class UItem> CurItem;
};

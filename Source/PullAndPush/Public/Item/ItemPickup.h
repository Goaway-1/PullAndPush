// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Actor.h"
#include "Interface/ItemPickupHandler.h"
#include "ItemPickup.generated.h"

DECLARE_DELEGATE(FItemPickupOnAction)

UCLASS()
class PULLANDPUSH_API AItemPickup : public AActor , public IItemPickupHandler
{
	GENERATED_BODY()
	
public:	
	AItemPickup();

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

protected:
	virtual void BeginPlay() override;

public:	
	FItemPickupOnAction OnPickupAction;

	// Settings when an item is created or used
	virtual void SetActiveItemPickup(bool IsSpawn, class UItemData* InItemDataAsset = nullptr, FVector InSpawnLocation = FVector(0.f)) override;
private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnRep_ChangeCurItemData();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeCurItemData, VisibleAnywhere, Category="Item")
	TWeakObjectPtr<class UItemData> CurItemData;

	// Is Item Spawn or Despawn
	UPROPERTY(Replicated)
	uint8 bIsSpawn:1;

	const FName CollisionName = TEXT("Item");
	const FName MeshCollisionName = "NoCollision";
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DeployableItem/DeployableStaticMeshItem.h"
#include "BlackHoleDeployableItem.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API ABlackHoleDeployableItem : public ADeployableStaticMeshItem
{
	GENERATED_BODY()

public:
	ABlackHoleDeployableItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void ActiveDeployableItem() override;

	// Set Acitve Black Hole
	void ActivateBlackHole();

	UFUNCTION()
	void AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DeleteOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, Category="DeployableItem")
	TObjectPtr<class USphereComponent> EventCollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "DeployableItem")
	TArray<TWeakObjectPtr<UPrimitiveComponent>> OverlapComponent;

	UPROPERTY(EditAnywhere, Category = "DeployableItem")
	float ForceStength;

	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | FX")
	TObjectPtr<class UNiagaraSystem> ExplosionEffect;

	uint8 bIsBlackHoleActived:1;
	uint8 bIsBlackHoleEffectActived:1;
};

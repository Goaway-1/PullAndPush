// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DeployableItem/DeployableItem.h"
#include "BlackHoleDeployableItem.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API ABlackHoleDeployableItem : public ADeployableItem
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
	UPROPERTY(EditAnywhere, Category="DeployableItem | BlackHole")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "DeployableItem | BlackHole")
	TArray<TWeakObjectPtr<UPrimitiveComponent>> OverlapComponent;

	UPROPERTY(EditAnywhere, Category = "DeployableItem | BlackHole")
	float ForceStength;

	uint8 bIsBlackHoleActived:1;
};

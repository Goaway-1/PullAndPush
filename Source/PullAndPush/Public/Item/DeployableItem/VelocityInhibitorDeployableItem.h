// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DeployableItem/DeployableStaticMeshItem.h"
#include "GameData/CharacterStat.h"
#include "VelocityInhibitorDeployableItem.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API AVelocityInhibitorDeployableItem : public ADeployableStaticMeshItem
{
	GENERATED_BODY()
public:
	AVelocityInhibitorDeployableItem();

protected:
	virtual void BeginPlay() override;
	virtual void ActiveDeployableItem() override;

	UFUNCTION()
	void AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DeleteOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem")
	TObjectPtr<class UBoxComponent> EventCollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | Stat")
	FCharacterStatModifier CharacterStatModifier;

	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | FX")
	TObjectPtr<class UNiagaraSystem> Effect;
};

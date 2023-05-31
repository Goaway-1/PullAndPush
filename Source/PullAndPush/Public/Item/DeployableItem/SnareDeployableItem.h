// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/CharacterStat.h"
#include "Item/DeployableItem/DeployableItem.h"
#include "SnareDeployableItem.generated.h"

/**
 * Snare the Character...
 */
UCLASS()
class PULLANDPUSH_API ASnareDeployableItem : public ADeployableItem
{
	GENERATED_BODY()
	
public:
	ASnareDeployableItem();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | Stat")
	FCharacterStatModifier CharacterStatModifier;

	const FName CollisionName = TEXT("Gimmick");
	uint8 bIsCollision : 1;
};

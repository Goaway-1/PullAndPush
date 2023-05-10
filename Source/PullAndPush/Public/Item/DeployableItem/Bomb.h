// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/DeployableItem/DeployableItem.h"
#include "Bomb.generated.h"

UCLASS()
class PULLANDPUSH_API ABomb : public ADeployableItem
{
	GENERATED_BODY()
	
public:	
	ABomb();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class URadialForceComponent> RadialForceComp;

private:
	void Explosion();

	// Explosion Timer
	void CheckIsExploded(float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item | FX")
	TObjectPtr<class UParticleSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Item | FX")
	TObjectPtr<class UMaterialInterface> ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Explosion")
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Explosion")
	float ExplosionTime;

	uint8 bIsExploded;
};

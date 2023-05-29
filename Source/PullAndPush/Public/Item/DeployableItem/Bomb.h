// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/DeployableItem/DeployableItem.h"
#include "GameData/CharacterStat.h"
#include "Bomb.generated.h"

UCLASS()
class PULLANDPUSH_API ABomb : public ADeployableItem
{
	GENERATED_BODY()
	
public:	
	ABomb();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class URadialForceComponent> RadialForceComp;

private:
	void Explosion();

protected:
	virtual void ActiveDeployableItem() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item | FX")
	TObjectPtr<class UParticleSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Item | FX")
	TObjectPtr<class UMaterialInterface> ExplodedMaterial;

	// Explosion Impulse Over Oneself
	const float SelfExplosionImpulse = 400.f;

	// Explosion Value other things
	const float ExplosionRadius = 250.f;
	const float ExplosionImpulse = 1000.f;

/** For Change Character Stat */
private:
	// Find overlapping characters and change their status
	void ModifyOverlappingCharacterStates();

	ECharacterStat ModifyCharacterStat;
	const float StatChangeDuration = 3.f;
};

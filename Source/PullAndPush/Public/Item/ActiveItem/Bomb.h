// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class PULLANDPUSH_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	ABomb();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class URadialForceComponent> RadialForceComp;

private:
	void Explosion();

	// Call Tick
	void CheckIsExploded(float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TObjectPtr<class UParticleSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TObjectPtr<class UMaterialInterface> ExplodedMaterial;

	uint8 bIsExploded;
	float ExplosionImpulse;
	float ExplosionTime;
	float DestoryTime;
};

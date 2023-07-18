// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DeployableItem.generated.h"

UCLASS()
class PULLANDPUSH_API ADeployableItem : public AActor
{
	GENERATED_BODY()
	
#pragma region DEFAULT
public:	
	ADeployableItem();

protected:
	virtual void BeginPlay() override;

#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere, Category = "DeployableItem")
	TObjectPtr<class USphereComponent> CollisionComp;

	const FName CollisionName = "PhysicsActor";
	const FName MeshCollisionName = "NoCollision";

/** Timer */
protected:
	UFUNCTION()
	virtual void ActiveDeployableItem();

	UFUNCTION()
	virtual void DestoryDeployableItem();

	// Automatically run timer when bIsAutoActive is enabled
	void SetActiveTimer();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | Time")
	float ActiveTime;

	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | Time")
	float DestoryTime;

	FTimerHandle ActiveHandler;
	FTimerHandle DestoryHandler;

	// Whether to run the timer automatically upon creation
	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | Time")
	uint8 bIsAutoActive : 1;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// The timer is activated when it bounce, which is used when "bIsAutoActive" is false.
	UFUNCTION()
	void OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};

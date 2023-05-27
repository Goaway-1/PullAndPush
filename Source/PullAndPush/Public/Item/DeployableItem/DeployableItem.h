// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	UPROPERTY(VisibleAnywhere, Category = "Item | Component")
	TObjectPtr<class UStaticMeshComponent> MeshComp;

/** Timer */
protected:
	UFUNCTION()
	virtual void ActiveDeployableItem();

	UFUNCTION()
	void DestoryDeployableItem();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item | Time")
	float ActiveTime;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Time")
	float DestoryTime;

	FTimerHandle ActiveHandler;
	FTimerHandle DestoryHandler;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;
};

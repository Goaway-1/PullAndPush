// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "GameFramework/Actor.h"
#include "Interface/DeployableItemHandler.h"
#include "DeployableItem.generated.h"

UCLASS()
class PULLANDPUSH_API ADeployableItem : public AActor, public IDeployableItemHandler
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

/** Handler Interface */
public:
	// @TODO : 여기서 넘겨주면 쉽게 처리 가능
	virtual void SetActivePhysicsAndCollision(bool InActive = true) override;

	//test
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangePhysicsAndCollision)
	uint8 bActivePhysicsAndCollision:1;

	UFUNCTION()
	void OnRep_ChangePhysicsAndCollision();
};

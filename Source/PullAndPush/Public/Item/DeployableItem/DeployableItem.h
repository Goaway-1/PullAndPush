// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:	
	virtual void Tick(float DeltaTime) override;

#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Time")
	float DestoryTime;


/** Handler */
public:
	virtual void SetActivePhysicsAndCollision(bool InActive = true) override;
};

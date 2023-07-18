// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DeployableItem/DeployableItem.h"
#include "DeployableSkeletalMeshItem.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API ADeployableSkeletalMeshItem : public ADeployableItem
{
	GENERATED_BODY()
public:
	ADeployableSkeletalMeshItem();

protected:
	void PlayAnimation();

	virtual void DestoryDeployableItem() override;
protected:
	UPROPERTY(VisibleAnywhere, Category = "DeployableItem")
	TObjectPtr<class USkeletalMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem")
	TObjectPtr<class UAnimationAsset> MeshAnimation;
};

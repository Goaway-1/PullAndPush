#pragma once

#include "CoreMinimal.h"
#include "Item/DeployableItem/DeployableItem.h"
#include "DeployableSkeletalMeshItem.generated.h"

/**
 * If DeployableItem doesn't have animation
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

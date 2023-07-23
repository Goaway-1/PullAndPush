#pragma once

#include "CoreMinimal.h"
#include "Item/DeployableItem/DeployableItem.h"
#include "DeployableStaticMeshItem.generated.h"

/**
 * If DeployableItem has an animation
 */
UCLASS()
class PULLANDPUSH_API ADeployableStaticMeshItem : public ADeployableItem
{
	GENERATED_BODY()
public:
	ADeployableStaticMeshItem();

protected:
	virtual void DestoryDeployableItem() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "DeployableItem")
	TObjectPtr<class UStaticMeshComponent> MeshComp;	
};

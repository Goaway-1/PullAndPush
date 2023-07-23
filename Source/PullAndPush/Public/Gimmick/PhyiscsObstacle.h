#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhyiscsObstacle.generated.h"

/**
* Gimmicks Affected by Physical Forces
*/
UCLASS()
class PULLANDPUSH_API APhyiscsObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	APhyiscsObstacle();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> DefaultSceneComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UPhysicsConstraintComponent> PhysicsConstraintComp;
};

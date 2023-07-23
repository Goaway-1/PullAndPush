#pragma once

#include "CoreMinimal.h"
#include "GameData/CharacterStat.h"
#include "Item/DeployableItem/DeployableSkeletalMeshItem.h"
#include "SnareDeployableItem.generated.h"

/**
 * Snare the Character...
 */
UCLASS()
class PULLANDPUSH_API ASnareDeployableItem : public ADeployableSkeletalMeshItem
{
	GENERATED_BODY()
	
public:
	ASnareDeployableItem();

protected:
	virtual void BeginPlay() override;

	virtual void ActiveDeployableItem() override;

private:
	UFUNCTION()
	void AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | VelocityInhibitor")
	TObjectPtr<class UBoxComponent> EventCollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "DeployableItem | Stat")
	FCharacterStatModifier CharacterStatModifier;

	uint8 bIsCollision : 1;
};

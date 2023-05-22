// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Components/ActorComponent.h"
#include "Interface/PickupActionHandler.h"
#include "ItemUsageComponent.generated.h"

/**
* Update Item Widget..
* @param	DataAsset		Item Data Asset
* @param	bool			Whether the update target is Passive or Active (Passive is True)
*/
DECLARE_DELEGATE_TwoParams(FOnItemWidgetUpdate, class UDataAsset*, const bool)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PULLANDPUSH_API UItemUsageComponent : public UActorComponent, public IPickupActionHandler
{
	GENERATED_BODY()

	public:
	UItemUsageComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/** Item */
public:
	void PickUpItem(class UItemData* ItemData);
	void TryToUsePassiveItem(class UItemData* ItemData);
	void TryToUseActiveItem();

private:
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UItemData> CurActiveItemData;

	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class AActor> CurDeployableItem;

	const FName ItemSocketName = "ItemSocket";
	uint8 bIsReadyToThrow : 1;

/** Throw Deployable Item */
public:
	void ThrowDeployableItem();

	FORCEINLINE bool GetIsReadyToThrow() { return bIsReadyToThrow; }

protected:
	// Create Projectile Item Path
	void CreatePredictedProjectilePath();
	void InitializeSplineMeshComponent(class USplineMeshComponent* InSplineMeshComponent, int32 SplineCount);
	void ClearSplineMeshComponents();

	UPROPERTY(VisibleAnywhere, Category = Spline)
	TObjectPtr<class USplineComponent> SplineComp;

	UPROPERTY(VisibleAnywhere, Category = Spline)
	TSet<TWeakObjectPtr<class USplineMeshComponent>> SplineMeshComps;

	UPROPERTY(EditAnywhere, Category = Spline)
	TObjectPtr<UStaticMesh> SplineStaticMesh;

	FVector LaunchVelocity;
	const int8 SplineMaxPathCnt = 10;
	const float AlphaLaunchPos = 50.f;		// Value added to current actor position for launch point
	const float DefaultLaunchForce = 800.f;
	const FVector DefaultLaunchForwardVector = FVector(0.f, 0.f, 0.5f);


/** Widget */
public:
	FOnItemWidgetUpdate& GetItemWidgetUpdateDelegate() { return OnItemWidgetUpdate; }

private:
	/**
	* Update Item Widget..
	* @param	DataAsset		Item Data Asset
	* @param	bool			Whether the update target is Passive or Active (Passive is True)
	*/
	FOnItemWidgetUpdate OnItemWidgetUpdate;
};
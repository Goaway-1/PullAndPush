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
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/** Item */
public:
	void PickUpItem(class UItemData* ItemData);
	void TryToUsePassiveItem(class UItemData* ItemData);
	void TryToUseActiveItem();

private:
	UFUNCTION()
	void OnRep_ChangeItemStaticMesh();

	// Item Data currently have
	UPROPERTY(Replicated, VisibleAnywhere, Category = Item)
	TObjectPtr<class UItemData> CurActiveItemData;

	// Item Data to Create
	UPROPERTY(Replicated, VisibleAnywhere, Category = Item)
	TObjectPtr<class UItemData> CurRequiredActiveItemData;

	UPROPERTY(Replicated, VisibleAnywhere, ReplicatedUsing = OnRep_ChangeItemStaticMesh, Category = Item)
	TObjectPtr<class UStaticMesh> CurDeployableItemStaticMesh;

	const FName ItemSocketName = "ItemSocket";
	uint8 bIsReadyToThrow : 1;

/** Throw Deployable Item */
public:
	/** Just Set Deployable Item Mesh (not Actor) */
	UFUNCTION(Server, Reliable)
	void ServerSetDeployableItemMesh(class UItemData* ActiveItemData = nullptr);

	/** Spawn & Throw Deployable Item on Server*/
	UFUNCTION(Server, Reliable)
	void ServerThrowDeployableItem(UClass* DeployableItemClass);

	void ThrowDeployableItem();

	FORCEINLINE bool GetIsReadyToThrow() { return bIsReadyToThrow; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<class UStaticMeshComponent> ItemStaticMeshComp;

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
	const float DefaultLaunchForce = 1300.f;
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

/** Timer */
protected:
	FTimerHandle AddTimer(class UItemData* PassiveItem, bool& bIsItemActivated);
	void RemoveTimer(FName TimerName);

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TMap<FName, FTimerHandle> TimerHandles;
};
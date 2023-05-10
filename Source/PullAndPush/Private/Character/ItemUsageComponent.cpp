// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ItemUsageComponent.h"
#include "Item/Item.h"
#include "Interface/ItemActionHandler.h"
#include "Interface/DeployableItemHandler.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

UItemUsageComponent::UItemUsageComponent()
	: 
	bIsReadyToThrow(0)
{
	PrimaryComponentTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
}
void UItemUsageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CreatePredictedProjectilePath();
}
void UItemUsageComponent::CreatePredictedProjectilePath()
{
	if (bIsReadyToThrow)
	{
		// Set 'StartPos, Launch Velocity'
		const FVector OwnerLocation = GetOwner()->GetActorLocation();
		const APlayerController* OwnerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
		const FVector OwnerForwardLocation = OwnerController->GetControlRotation().Vector();

		const FVector StartPos = OwnerLocation + (OwnerForwardLocation * AlphaLaunchPos);
		LaunchVelocity = (OwnerForwardLocation + DefaultLaunchForwardVector) * DefaultLaunchForce;

		// Try to draw PreditProjectilePath
		FHitResult HitResult;
		TArray<FVector> OutPathPositions;
		FVector OutLastTraceDestination;
		TArray<AActor*> ActorsToIgnore;
		bool bIsCanDrawPath = UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(GetWorld(), HitResult, OutPathPositions, OutLastTraceDestination, StartPos,
			LaunchVelocity, true, 1.f, ECollisionChannel::ECC_WorldDynamic, true, ActorsToIgnore, EDrawDebugTrace::None, 1.f);
	
		// Draw PreditProjectilePath
		ClearSplineMeshComponents();
		
		// Set Spline Point
		for (int32 idx = 0; idx < OutPathPositions.Num() - 1; idx++)
		{
			SplineComp->AddSplinePointAtIndex(OutPathPositions[idx], idx, ESplineCoordinateSpace::World);
			if(idx > SplineMaxPathCnt) break;
		}

		// Create Spline Mesh
		for (int32 SplineCount = 0; SplineCount < SplineComp->GetNumberOfSplinePoints() - 1; ++SplineCount)
		{
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			InitializeSplineMeshComponent(SplineMeshComponent, SplineCount);
			SplineMeshComps.Add(SplineMeshComponent);
		}
	}
}
void UItemUsageComponent::InitializeSplineMeshComponent(USplineMeshComponent* InSplineMeshComponent, int32 SplineCount)
{
	ensure(SplineComp);

	InSplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
	InSplineMeshComponent->SetStaticMesh(SplineStaticMesh);
	InSplineMeshComponent->SetMobility(EComponentMobility::Movable);
	InSplineMeshComponent->RegisterComponentWithWorld(GetWorld());
	InSplineMeshComponent->AttachToComponent(SplineComp, FAttachmentTransformRules::KeepRelativeTransform);
	InSplineMeshComponent->SetStartScale(FVector2D(5.f));
	InSplineMeshComponent->SetEndScale(FVector2D(5.f));

	const FVector StartPoint = SplineComp->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
	const FVector StartTangent = SplineComp->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
	const FVector EndPoint = SplineComp->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
	const FVector EndTangent = SplineComp->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
	InSplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

	InSplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void UItemUsageComponent::ClearSplineMeshComponents()
{
	SplineComp->ClearSplinePoints(true);
	for (auto& SplineMesh : SplineMeshComps)
	{
		if (SplineMesh.IsValid())
		{
			SplineMesh->DestroyComponent();
		}
	}
	SplineMeshComps.Empty();
}
void UItemUsageComponent::PickUpItem(UItem* ItemData)
{
	check(ItemData);

	// Play "Active or Passive" Action
	if (!ItemData->CheckIsActiveItem()) {
		TryToUsePassiveItem(ItemData);
	}
	else {
		CurActiveItemData = ItemData;

		// Show 'Passive Widget'
		OnItemWidgetUpdate.Execute(CurActiveItemData, false);
	}
}
void UItemUsageComponent::ThrowDeployableItem()
{
<<<<<<< Updated upstream
	if (!CurDeployableItem) return;

	PPLOG(Log,TEXT("Throw Item"));
	
	// Detach
	CurDeployableItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
=======
	if (!CurDeployableItem.IsValid()) return;
	
	// Set Location and Detach
	CurDeployableItem.Get()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
>>>>>>> Stashed changes

	// Set Deployable Item Physics and Collision
	TScriptInterface<class IDeployableItemHandler> CurItemHandler = CurDeployableItem;
	if (CurItemHandler.GetInterface())
	{	
		CurItemHandler->SetActivePhysicsAndCollision();
	}

	// Throw Deployable Item
	UStaticMeshComponent* DeployableMesh = Cast<UStaticMeshComponent>(CurDeployableItem.Get()->GetRootComponent());
	DeployableMesh->SetAllPhysicsLinearVelocity(LaunchVelocity * 2.5f);
	ClearSplineMeshComponents();

	CurDeployableItem = nullptr;
	bIsReadyToThrow = false;
}
void UItemUsageComponent::TryToUseActiveItem()
{
	if(!CurActiveItemData) return;

	PPLOG(Log, TEXT("UseActionItem"));
	TScriptInterface<class IItemActionHandler> CurItemAction = CurActiveItemData;
	if (CurItemAction.GetInterface())
	{
		// Spawn
		TSubclassOf<class AActor> DeployableItemClass = CurItemAction->GetSpawnItemClass();
		CurDeployableItem = GetWorld()->SpawnActor(DeployableItemClass);
		CurDeployableItem->SetActorLocation(GetOwner()->GetActorLocation());

		// Attach
		CurDeployableItem->AttachToActor(GetOwner(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemSocketName);

		// Hide 'Passive Widget'
<<<<<<< Updated upstream
		OnItemWidgetUpdate.Execute(CurActiveItemData, false);
=======
		OnItemWidgetUpdate.Execute(CurActiveItemData.Get(), false);

		bIsReadyToThrow = true;
		CurActiveItemData = nullptr;
>>>>>>> Stashed changes
	}
}
void UItemUsageComponent::TryToUsePassiveItem(UItem* ItemData)
{
	TScriptInterface<class IItemActionHandler> CurItemAction = ItemData;
	if (CurItemAction.GetInterface()) 
	{
		CurItemAction->UseItem(GetOwner());

		// Show 'Active Widget'
		OnItemWidgetUpdate.Execute(ItemData, true);
	}
}
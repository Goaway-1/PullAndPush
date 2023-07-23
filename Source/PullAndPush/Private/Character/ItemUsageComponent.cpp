#include "Character/ItemUsageComponent.h"
#include "Item/ItemData/ItemData.h"
#include "Interface/ItemDataHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Net/UnrealNetwork.h"

UItemUsageComponent::UItemUsageComponent()
	:
	bIsReadyToThrow(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	ItemStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMeshComp"));
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}
void UItemUsageComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set Item Static Mesh Component
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		ItemStaticMeshComp->SetWorldLocation(OwnerCharacter->GetMesh()->GetSocketLocation(ItemSocketName));
		ItemStaticMeshComp->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemSocketName);
		ItemStaticMeshComp->SetVisibility(false);
	}
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
			if (idx > SplineMaxPathCnt) break;
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
void UItemUsageComponent::PickUpItem(UItemData* ItemData)
{
	check(ItemData);

	// Play "Active or Passive" Action
	if (!ItemData->CheckIsActiveItem()) {
		TryToUsePassiveItem(ItemData);
	}
	else {
		CurActiveItemData = ItemData;

		// Show 'Active Widget'
		OnItemWidgetUpdate.ExecuteIfBound(CurActiveItemData, false);	
	}
}
void UItemUsageComponent::ServerSetDeployableItemMesh_Implementation(UItemData* ActiveItemData)
{
	TScriptInterface<class IItemDataHandler> CurItemAction = ActiveItemData;
	if (CurItemAction.GetInterface())
	{
		CurDeployableItemStaticMesh = CurItemAction->GetDeployStaticMesh();
		ItemStaticMeshComp->SetStaticMesh(CurDeployableItemStaticMesh);
		ItemStaticMeshComp->SetVisibility(true);
	}
	else
	{
		CurDeployableItemStaticMesh = nullptr;
		ItemStaticMeshComp->SetVisibility(false);
	}
}
void UItemUsageComponent::ThrowDeployableItem()
{
	if (!CurRequiredActiveItemData) return;

	TSubclassOf<class AActor> DeployableItemClass = CurRequiredActiveItemData->GetDeployItemClass();
	ServerThrowDeployableItem(DeployableItemClass);
	ClearSplineMeshComponents();

	bIsReadyToThrow = false;
	CurRequiredActiveItemData = nullptr;
}
void UItemUsageComponent::ServerThrowDeployableItem_Implementation(UClass* DeployableItemClass)
{
	// Spawn & Throw Deployable Item
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	const FRotator SpawnRotation = (PlayerController->GetControlRotation().Vector() + DefaultLaunchForwardVector).GetSafeNormal().Rotation();
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	// Spawn the projectile at the muzzle
	GetWorld()->SpawnActor<AActor>(DeployableItemClass, SpawnLocation, SpawnRotation);
	ServerSetDeployableItemMesh();
}
void UItemUsageComponent::TryToUseActiveItem()
{
	if (!CurActiveItemData) return;

	// Set Item Mesh 
	ServerSetDeployableItemMesh(CurActiveItemData);

	// Hide 'Active Widget'
	OnItemWidgetUpdate.ExecuteIfBound(nullptr, false);
	bIsReadyToThrow = true;
	CurRequiredActiveItemData = CurActiveItemData;
	CurActiveItemData = nullptr;
}
void UItemUsageComponent::TryToUsePassiveItem(UItemData* ItemData)
{
	TScriptInterface<class IItemDataHandler> CurItemAction = ItemData;
	if (CurItemAction.GetInterface())
	{
		bool bItemAlreadyActivated;
		FTimerHandle Handler = AddTimer(ItemData, bItemAlreadyActivated);
		CurItemAction->UsePassiveItem(GetOwner(), Handler, bItemAlreadyActivated);

		// Show 'Passive Widget'
		OnItemWidgetUpdate.ExecuteIfBound(ItemData, true);
	}
}
FTimerHandle UItemUsageComponent::AddTimer(class UItemData* PassiveItem, bool& bIsItemActivated)
{
	// Check handler already exists
	const FName TimerName = FName(PassiveItem->GetItemName());
	const float Duration = PassiveItem->GetDurationTime();
	if (TimerHandles.Contains(TimerName))
	{
		RemoveTimer(PassiveItem);
		PPLOG(Log, TEXT("%s Item Time is already exists!"),*TimerName.ToString());
		bIsItemActivated = true;
	}
	else
	{
		PPLOG(Log, TEXT("Add Item Timer, Item Name : %s, Duration : %f"), *TimerName.ToString(), Duration);
		bIsItemActivated = false;
	}

	// Set Timer & Handler
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateUObject(this, &UItemUsageComponent::RemoveTimer, PassiveItem), Duration, false);
	
	// Add the timer handle to the array
	TimerHandles.Add(TimerName, Handle);

	return Handle;
}
void UItemUsageComponent::RemoveTimer(class UItemData* PassiveItem)
{
	const FName TimerName = FName(PassiveItem->GetItemName());
	FTimerHandle* Handle = TimerHandles.Find(TimerName);
	if (Handle->IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(*Handle);
		TimerHandles.Remove(TimerName);

		PPLOG(Log, TEXT("Remove Item Handle : %s"), *TimerName.ToString());
	}

	PassiveItem->EndPassiveItem();
}
void UItemUsageComponent::OnRep_ChangeItemStaticMesh()
{
	if (CurDeployableItemStaticMesh)
	{
		ItemStaticMeshComp->SetStaticMesh(CurDeployableItemStaticMesh);
		ItemStaticMeshComp->SetVisibility(true);
	}
	else
	{
		ItemStaticMeshComp->SetVisibility(false);
	}
}
void UItemUsageComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemUsageComponent, CurActiveItemData);
	DOREPLIFETIME(UItemUsageComponent, CurRequiredActiveItemData);
	DOREPLIFETIME(UItemUsageComponent, CurDeployableItemStaticMesh);
}
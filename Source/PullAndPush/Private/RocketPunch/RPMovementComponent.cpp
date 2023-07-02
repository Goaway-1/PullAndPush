#include "RocketPunch/RPMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RocketPunch/RocketPunch.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

URPMovementComponent::URPMovementComponent()
	:
	bIsReturn(false), bIsLaunch(false), bIsForceReturn(false),
	ReturnMoveSpeed(35.f), MaxMoveSpeed(25.f), MinMoveSpeed(15.f), MaxDistance(1000.f), MinDistance(500.f)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	CurMoveSpeed = MinMoveSpeed;
}
void URPMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	InitSetting();
}
void URPMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckMovement();
}
void URPMovementComponent::InitSetting()
{
	Owner = Cast<ARocketPunch>(GetOwner());
	Owner->SetActorEnableCollision(false);
	Owner->SetMeshVisibility(false);
	Owner->SetActorTickEnabled(false);
}
void URPMovementComponent::SetPreDistance(bool IsReturn, float InTargetDistance)
{
	StartLoc = Owner->GetActorLocation();
	EndLoc = (!IsReturn) ? StartLoc + (Owner->GetActorForwardVector() * InTargetDistance) : CasterActor->GetActorLocation();
	PreDistance = (EndLoc - StartLoc).Size();
}
void URPMovementComponent::CheckMovement()
{
	if (bIsLaunch) {
		if (bIsReturn) UpdateRotation();
		UpdateLocation();
	}
}
void URPMovementComponent::Launch(const float& ForceAlpha, AActor* InCasterActor, const FVector& InVec, const FRotator& InRot, const float& AlphaSpeed, const float& AlphaRange)
{
	if (CasterActor == nullptr) CasterActor = InCasterActor;

	// Rocket Punch Setting
	Owner->SetActorLocationAndRotation(InVec, InRot);
	Owner->SetActorEnableCollision(true);
	Owner->SetMeshVisibility(true);
	Owner->SetActorTickEnabled(true);
	Owner->SetCollisionSimulatePhysics(true);

	// Movement Setting
	bIsLaunch = true;
	bIsReturn = false;
	bIsForceReturn = false;

	// Target Distance & Speed by Lerp + Plus Alpha Value...
	const float LerpDistance = FMath::Lerp(MinDistance, MaxDistance, ForceAlpha) * AlphaRange;
	const float LerpMoveSpeed = FMath::Lerp(MinMoveSpeed, MaxMoveSpeed, ForceAlpha) * AlphaSpeed;

	SetPreDistance(false, LerpDistance);
	SetCurMoveSpeed(LerpMoveSpeed);

	UE_LOG(LogTemp, Log, TEXT("[URPMovementComponent] ForceAlpha : %f, Distance : %f , Speed : %f"), ForceAlpha, LerpDistance, LerpMoveSpeed);
}
void URPMovementComponent::UpdateLocation()
{
	CurLocation = Owner->GetActorLocation() + (Owner->GetActorForwardVector() * CurMoveSpeed);
	Owner->SetActorLocation(CurLocation);

	// Check is nearby target pos
	CurDistance = (EndLoc - Owner->GetActorLocation()).Size();
	if (GetIsForceReturn() || (PreDistance <= CurDistance && CasterActor)) {
		SetPreDistance(true);

		// Return or Invisible
		if (!GetIsReturn()) {
			bIsReturn = true;
			bIsForceReturn = false;
			Owner->SetActorEnableCollision(false);
			Owner->SetCollisionSimulatePhysics(false);
			SetCurMoveSpeed(ReturnMoveSpeed);
		}
		else {
			bIsLaunch = false;
			SetCanLaunch(true);
			Owner->SetActorLocation(FVector(999.f));		// Set Location Safe Place
			Owner->SetMeshVisibility(false);
			Owner->SetActorTickEnabled(false);
		}
	}
	else PreDistance = CurDistance;
}
void URPMovementComponent::OnRep_ChangeLocation()
{
	Owner->SetActorLocation(CurLocation);
}
void URPMovementComponent::UpdateRotation() 
{
	CurRotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), CasterActor->GetActorLocation());
	Owner->SetActorRotation(CurRotation);
	EndLoc = CasterActor->GetActorLocation() - ((CasterActor->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal() * 150.f);
}
void URPMovementComponent::OnRep_ChangeRotation()
{
	Owner->SetActorRotation(CurRotation);
}
void URPMovementComponent::SetCanLaunch(const bool& Val)
{
	UE_LOG(LogTemp, Log, TEXT("[RPMovementComponent] OnReturn Delegate is called!"));
	OnReturn.Execute(Val);
}
void URPMovementComponent::SetIsForceReturn(const bool& Val)
{
	bIsForceReturn = Val;
}
void URPMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPMovementComponent, CurLocation);
	DOREPLIFETIME(URPMovementComponent, CurRotation);
}
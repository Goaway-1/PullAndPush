#include "RPMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RocketPunch.h"

URPMovementComponent::URPMovementComponent()
	:
	bIsReturn(false), bIsLaunch(false), CurMoveSpeed(MinMoveSpeed), bIsForceReturn(false)
{
	PrimaryComponentTick.bCanEverTick = true;

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
	Owner = GetOwner();
	Owner->SetActorEnableCollision(false);
	Owner->SetActorHiddenInGame(true);
	Owner->SetActorTickEnabled(false);
}
void URPMovementComponent::CheckMovement()
{
	if (bIsLaunch) {
		if (bIsReturn) UpdateRotation();
		UpdateLocation();
	}
}
void URPMovementComponent::Launch(const float& Force, AActor* InOwnerPlayerActor, const FVector& InVec, const FRotator& InRot)
{
	if (OwnerPlayerActor == nullptr) OwnerPlayerActor = InOwnerPlayerActor;

	Owner->SetActorLocationAndRotation(InVec, InRot);
	Owner->SetActorEnableCollision(true);
	Owner->SetActorHiddenInGame(false);
	Owner->SetActorTickEnabled(true);

	StartLoc = Owner->GetActorLocation();
	EndLoc = StartLoc + (Owner->GetActorForwardVector() * DefaultForce * Force);
	PreDistance = (EndLoc - StartLoc).Size();

	bIsLaunch = true;
	bIsReturn = false;
	bIsForceReturn = false;
	SetCurMoveSpeed(MinMoveSpeed);

	UE_LOG(LogTemp, Log, TEXT("Launch RocketPunch!!"));
}
void URPMovementComponent::UpdateLocation() {
	Owner->SetActorLocation(Owner->GetActorLocation() + (Owner->GetActorForwardVector() * CurMoveSpeed));

	// Check is nearby target pos
	CurDistance = (EndLoc - Owner->GetActorLocation()).Size();
	if (GetIsForceReturn() || (PreDistance <= CurDistance && OwnerPlayerActor)) {
		StartLoc = Owner->GetActorLocation();
		EndLoc = OwnerPlayerActor->GetActorLocation();
		PreDistance = (EndLoc - Owner->GetActorLocation()).Size();

		// Return or Invisible
		if (!GetIsReturn()) {
			bIsReturn = true;
			bIsForceReturn = false;
			Owner->SetActorEnableCollision(false);
			SetCurMoveSpeed(MaxMoveSpeed);
		}
		else {
			bIsLaunch = false;
			SetCanLaunch(true);
			Owner->SetActorHiddenInGame(true);
			Owner->SetActorTickEnabled(true);
		}
	}
	else PreDistance = CurDistance;
}
void URPMovementComponent::UpdateRotation() 
{
	const FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), OwnerPlayerActor->GetActorLocation());
	Owner->SetActorRotation(NewRot);
	EndLoc = OwnerPlayerActor->GetActorLocation();
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

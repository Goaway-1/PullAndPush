// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPMovementComponent.generated.h"

DECLARE_DELEGATE_OneParam(FRPMovementComponentOnReturn, const bool&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API URPMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReadyToLaunch(const float& Force);

	// Make it possible to attack again
	FRPMovementComponentOnReturn OnReturn;

private:
	// Is RocketPunch is return to Player? Or Launched?
	bool bIsReturn;
	bool bIsLaunch;

	// Decide to Return or End
	double PreDistance;
	double CurDistance;
	FVector StartLoc;
	FVector EndLoc;

	// Speed
	const double DefaultForce = 1000.f;
	const double MaxMoveSpeed = 25.f;
	const double MinMoveSpeed = 15.f;
	double CurMoveSpeed;

	UPROPERTY()
	TObjectPtr<class ARocketPunch> OwnerActor;

	void Launch();
	void UpdateRotation();
	void CheckMovement();
	void InitSetting();
	const AActor* GetRPOwner();

	// Related to FRPMovementComponentOnReturn
	UFUNCTION()
	void SetCanLaunch(const bool& Val);

	FORCEINLINE bool GetIsReturn() { return bIsReturn; }
	FORCEINLINE void SetIsReturn(const bool Val) { bIsReturn = Val; }
	FORCEINLINE bool GetIsLaunch() { return bIsLaunch; }
	FORCEINLINE void SetIsLaunch(const bool Val) { bIsLaunch = Val; }
	FORCEINLINE void SetCurMoveSpeed(const double Val) { CurMoveSpeed = Val; }
};

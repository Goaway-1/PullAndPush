// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPMovementComponent.generated.h"


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

	void Launch();
	void UpdateRotation();
	void CheckMovement();
	void InitSetting();

	UPROPERTY()
	TObjectPtr<class ARocketPunch> OwnerActor;

	const AActor* GetRPOwner();

	FORCEINLINE bool GetIsReturn() { return bIsReturn; }
	FORCEINLINE void SetIsReturn(const bool Val) { bIsReturn = Val; }
	FORCEINLINE bool GetIsLaunch() { return bIsLaunch; }
	FORCEINLINE void SetIsLaunch(const bool Val) { bIsLaunch = Val; }
	FORCEINLINE void SetCurMoveSpeed(const double Val) { CurMoveSpeed = Val; }
};

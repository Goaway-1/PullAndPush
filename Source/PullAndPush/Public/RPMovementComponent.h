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

	void Launch(const float& Force, AActor* InOwnerPlayerActor, const FVector& InVec, const FRotator& InRot);

	// Make it possible to attack again
	FRPMovementComponentOnReturn OnReturn;

	UFUNCTION()
	void SetIsForceReturn(const bool& Val);

private:
	// Is RocketPunch is return to Player? Or Launched?
	UPROPERTY(VisibleAnywhere, Category="Info", meta=(AllowPrivateAccess = "true"))
	uint8 bIsReturn:1;

	UPROPERTY(VisibleAnywhere, Category = "Info", meta = (AllowPrivateAccess = "true"))
	uint8 bIsLaunch:1;

	UPROPERTY(VisibleAnywhere, Category = "Info", meta = (AllowPrivateAccess = "true"))
	uint8 bIsForceReturn : 1;

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
	TObjectPtr<class ARocketPunch> Owner;			// RocketPunch

	UPROPERTY()
	TObjectPtr<class AActor> OwnerPlayerActor;		// RocketPunch의 소유자

	void UpdateLocation();
	void UpdateRotation();
	void CheckMovement();
	void InitSetting();

	// Related to FRPMovementComponentOnReturn
	UFUNCTION()
	void SetCanLaunch(const bool& Val);

	FORCEINLINE bool GetIsReturn() const { return bIsReturn; }
	FORCEINLINE void SetIsReturn(const bool Val) { bIsReturn = Val; }
	FORCEINLINE bool GetIsLaunch() const { return bIsLaunch; }
	FORCEINLINE void SetIsLaunch(const bool Val) { bIsLaunch = Val; }
	FORCEINLINE void SetCurMoveSpeed(const double Val) { CurMoveSpeed = Val; }
	FORCEINLINE bool GetIsForceReturn() const { return bIsForceReturn; }

};

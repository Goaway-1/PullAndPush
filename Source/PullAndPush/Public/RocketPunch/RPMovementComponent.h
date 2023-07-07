// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/CharacterStat.h"
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

	void Launch(const float& ForceAlpha, AActor* InCasterActor, const FVector& InVec, const FRotator& InRot, FPassiveStat InPassiveStat);

	// Make it possible to attack again
	FRPMovementComponentOnReturn OnReturn;

	UFUNCTION()
	void SetIsForceReturn(bool Val);

private:
	// Is RocketPunch is return to Player? Or Launched?
	uint8 bIsReturn:1;
	uint8 bIsLaunch:1;
	uint8 bIsForceReturn : 1;

	// Decide to Return or End
	UPROPERTY(Transient, VisibleAnywhere, Category = "Movement | Distance")
	float PreDistance;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Movement | Distance")
	float CurDistance;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Movement | Location")
	FVector StartLoc;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Movement | Location")
	FVector EndLoc;

	// Max Min Speed & Distance
	UPROPERTY(EditAnyWhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ReturnMoveSpeed;

	UPROPERTY(EditAnyWhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxMoveSpeed;

	UPROPERTY(EditAnyWhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MinMoveSpeed;

	UPROPERTY(EditAnyWhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxDistance;

	UPROPERTY(EditAnyWhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MinDistance;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Movement")
	float CurMoveSpeed;

	UPROPERTY()
	TObjectPtr<class ARocketPunch> Owner;			// RocketPunch

	UPROPERTY()
	TObjectPtr<class AActor> CasterActor;			// RocketPunch의 소유자

	void UpdateLocation();
	void UpdateRotation();
	void CheckMovement();
	void InitSetting();
	void SetPreDistance(bool IsReturn, float InTargetDistance = 0.f);

	// Related to FRPMovementComponentOnReturn
	UFUNCTION()
	void SetCanLaunch(const bool& Val);

	FORCEINLINE bool GetIsReturn() const { return bIsReturn; }
	FORCEINLINE void SetIsReturn(const bool Val) { bIsReturn = Val; }
	FORCEINLINE bool GetIsLaunch() const { return bIsLaunch; }
	FORCEINLINE void SetIsLaunch(const bool Val) { bIsLaunch = Val; }
	FORCEINLINE void SetCurMoveSpeed(const double Val) { CurMoveSpeed = Val; }
	FORCEINLINE bool GetIsForceReturn() const { return bIsForceReturn; }

private:
	UFUNCTION()
	void OnRep_ChangeLocation();

	UFUNCTION()
	void OnRep_ChangeRotation();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeLocation)
	FVector CurLocation;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeRotation)
	FRotator CurRotation;
};

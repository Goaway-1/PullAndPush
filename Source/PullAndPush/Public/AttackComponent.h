// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FAttackComponentOnChangedSignature, const float&, const float&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryLaunch();
	void ChargingLaunch();
	void EndLaunch();

	// Charging Value
	float ChargingTime;
	bool bIsCharging;
	bool bIsChangedSpeed;
	const float DecideChargingTimeSec = 0.2f;
	const float CanLaunchedTime = 1.0f;

	// PlayalbeCharacter.h
	const float MaxMoveSpeed = 600.f;
	const float MinMoveSpeed = 200.f;
	const float MaxJumpVelocity = 420.f;
	const float MinJumpVelocity = 210.f;

	// Delegate
	FAttackComponentOnChangedSignature OnCharging;
	void ChangeMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity);

		
};

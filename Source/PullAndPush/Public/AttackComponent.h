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

	// Change Player Move Speed & Camera View
	FAttackComponentOnChangedSignature OnCharging;

private:
	// Charging Value
	float ChargingTime;
	bool bIsCharging;
	bool bIsChangeValue;
	const float DecideChargingTime = 0.2f;
	const float CanLaunchedTime = 1.0f;
	const float MaxChargingTime = 2.5f;
	const float MinChargingTime = 0.2f;

	const float MaxMoveSpeed = 600.f;
	const float MinMoveSpeed = 200.f;
	const float MaxJumpVelocity = 420.f;
	const float MinJumpVelocity = 210.f;

	void ChangeMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity);
	
	// Delegate : Make it possible to attack again
	bool bIsCanLaunch;

	UFUNCTION()
	void SetCanLaunch(const bool& Val);

	TObjectPtr<class ACharacter> OwnerCharacter;
	USkeletalMeshSocket const* RocketPunchSocket;

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ARocketPunch> RocketPunchClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class ARocketPunch> RocketPunch;
};
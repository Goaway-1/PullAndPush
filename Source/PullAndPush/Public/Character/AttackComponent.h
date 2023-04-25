// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

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

	bool TryLaunch();
	void ChargingLaunch();
	void EndLaunch(bool bIsPush);

private:
	// Charging Value
	float ChargingTime;
	uint8 bIsCharging:1;
	uint8 bIsChangeValue:1;
	const float DecideChargingTime = 0.2f;
	const float CanLaunchedTime = 1.0f;
	const float MaxChargingTime = 2.5f;
	const float MinChargingTime = 0.2f;

	void ChangeMovementSpeed(const bool& IsCharging);
	
	// Delegate : Make it possible to attack again
	uint8 bIsCanLaunch:1;

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
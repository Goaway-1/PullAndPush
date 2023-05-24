// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
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

	virtual void ReadyForReplication() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryLaunch();
	void ChargingLaunch();
	void EndLaunch(bool bIsPush);
private:
	void SpawnRocketPunch();

	UFUNCTION(Server, Reliable)
	void ServerSpawnRocketPunch();

	void ChangeMovementSpeed(const bool& IsCharging);

	UFUNCTION(Client, Reliable)
	void ClientSetCanLaunch(const bool Val);
private:
	// Charging Value
	UPROPERTY(Transient)
	float ChargingTime;

	const FName RocketPunchSocketName = "RocketPunch";
	uint8 bIsCharging:1;
	uint8 bIsChangeValue:1;
	const float DecideChargingTime = 0.2f;
	const float CanLaunchedTime = 1.0f;
	const float MaxChargingTime = 2.5f;
	const float MinChargingTime = 0.2f;
		
	// Delegate : Make it possible to attack again
	uint8 bIsCanLaunch:1;

	TWeakObjectPtr<class ACharacter> OwnerCharacter;

	USkeletalMeshSocket const* RocketPunchSocket;

protected:
	UPROPERTY(EditAnywhere, Category = "RocketPunch")
	TSubclassOf<class ARocketPunch> RocketPunchClass;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "RocketPunch")
	TObjectPtr<class ARocketPunch> RocketPunch;

/** Item Alpha Value */
public:
	FORCEINLINE void SetRPAlphaSpeed(const float& AlphaSpeed) { RPAlphaSpeed = AlphaSpeed; }
	FORCEINLINE void SetRPAlphaRange(const float& AlphaRange) { RPAlphaRange = AlphaRange; }
	FORCEINLINE void SetRPAlphaSize(const float& AlphaSize) { RPAlphaSize = AlphaSize; }

private:
	// @TODO : VisibleAnywhere·Î ¼öÁ¤
	UPROPERTY(Transient, EditAnywhere, Category = "RocketPunch | Alpha Value")
	float RPAlphaSpeed;

	UPROPERTY(Transient, EditAnywhere, Category = "RocketPunch | Alpha Value")
	float RPAlphaRange;

	UPROPERTY(Transient, EditAnywhere, Category = "RocketPunch | Alpha Value")
	float RPAlphaSize;
};
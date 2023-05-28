// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DELEGATE(FMoveSpeedChanged)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PULLANDPUSH_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/** Movement Speed */
private:
	// Call On Tick..
	void UpdateCurrnentMovementSpeed();

	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat | Movement")
	float CurrentMoveSpeed;

	std::atomic<float> PendingMoveSpeed;

	const float DefaultMoveSpeed = 600.f;
	const float DefaultJumpVelocity = 420.f;

public:
	// Try to set Movement Speed (+ Delta Speed Value)
	bool ModifyMoveSpeed(const float DeltaSpeed = 0.f);

	FORCEINLINE float GetMoveSpeed() {return CurrentMoveSpeed;}
	FORCEINLINE float GetJumpVelocity() {return DefaultJumpVelocity; }

	// if MoveSpeed is Changed..
	FMoveSpeedChanged OnMoveSpeedChanged;

/** Rocket Punch Values */
public:
	FORCEINLINE void SetRocketPunchSpeed(const float& DeltaSpeed) { RocketPunchSpeed = DeltaSpeed; }
	FORCEINLINE void SetRocketPunchRange(const float& DeltaRange) { RocketPunchRange = DeltaRange; }
	FORCEINLINE void SetRocketPunchScale(const float& DeltaSize) { RocketPunchScale = DeltaSize; }
	FORCEINLINE float GetRocketPunchSpeed() { return RocketPunchSpeed; }
	FORCEINLINE float GetRocketPunchRange() { return RocketPunchRange; }
	FORCEINLINE float GetRocketPunchScale() { return RocketPunchScale; }

private:
	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat | RocketPunch")
	float RocketPunchSpeed;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat | RocketPunch")
	float RocketPunchRange;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat | RocketPunch")
	float RocketPunchScale;
};

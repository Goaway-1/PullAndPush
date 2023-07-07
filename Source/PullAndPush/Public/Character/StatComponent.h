// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Components/ActorComponent.h"
#include "GameData/CharacterStat.h"
#include "Interface/CharacterStatHandler.h"
#include "StatComponent.generated.h"

DECLARE_DELEGATE(FMoveSpeedChanged)

/**
* Update Stat Widget..
* @param	FString					Stat Name
* @param	UMaterialInterface		Whether to create or delete 
*/
DECLARE_DELEGATE_TwoParams(FOnUpdateStatWidget, const FString&, UMaterialInterface* /** null is Delete */)

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
public:
	// Try to set Movement Speed (+ Delta Speed Value)
	bool ModifyMoveSpeed(const float DeltaSpeed = 0.f);

	FORCEINLINE float GetMoveSpeed() { return CurrentMoveSpeed; }
	FORCEINLINE float GetJumpVelocity() { return DefaultJumpVelocity; }

	// if MoveSpeed is Changed..
	FMoveSpeedChanged OnMoveSpeedChanged;

private:
	// Call On Tick..
	void UpdateCurrnentMovementSpeed();

	UPROPERTY(Transient, VisibleAnywhere, Category = "Stat | Movement")
	float CurrentMoveSpeed;

	std::atomic<float> PendingMoveSpeed;

	const float DefaultMoveSpeed = 600.f;
	const float DefaultJumpVelocity = 420.f;

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

/** Stat System */
#pragma region STATSYSTEM
public:
	void SetPassiveStat(FPassiveStat InPassiveStat);
	FPassiveStat GetPassiveStat();

	UFUNCTION()
	void EnableStatFlag(ECharacterStat InFlag, float ChangeDuration);

	UFUNCTION()
	void DisableStatFlag(ECharacterStat InFlag);

	UFUNCTION()
	bool IsStatFlagSet(ECharacterStat InFlag);	// Is Flag bit enable?

protected:
	// 'Create or Delete' Stat Timer
	void CreateStatFlagTimer(ECharacterStat InFlag, float ChangeDuration);
	void RemoveStatFlagTimer(ECharacterStat InFlag);


protected:
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	FPassiveStat DefaultPassiveStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Flag", meta = (Bitmask, BitmaskEnum = "/Script/PullAndPush.ECharacterStat"))
	uint8 StatFlags;

	UPROPERTY(VisibleAnywhere, Category = "Stat | Flag")
	TMap<FString, FTimerHandle> StatFlagHandles;
#pragma endregion

/** Stat Widget */
#pragma region WIDGET
public:
	FOnUpdateStatWidget OnUpdateStatWidget;

protected:
	// 'Create or Delete' Stat Widget
	void UpdateStatWidget(FString StatName, bool IsCreateWidget);

	// Get Stat Material
	UMaterialInterface* GetMaterialForCharacterStat(FString StatName);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Stat | Flag")
	TObjectPtr<class UDataTable> StatMartialTable;
#pragma endregion
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class EPlayerAttackCondition : uint8 {
	EPAC_Idle = 0		UMETA(DisplayName = "Idle"),
	EPAC_Charging		UMETA(DisplayName = "Charging")
};

UCLASS()
class PULLANDPUSH_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayableCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE EPlayerAttackCondition GetPlayerAttackCondition() {return PlayerAttackCondition;}
	FORCEINLINE void SetPlayerAttackCondition(const EPlayerAttackCondition& NewPlayerAttackCondition) { PlayerAttackCondition = NewPlayerAttackCondition;}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneeComp;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* ChargingSpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UAttackComponent> AttackComp;

private:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void TryLaunch();
	void ChargingLaunch();
	void EndLaunch();
	void SetMovementSpeed(const float& NewMoveSpeed, const float& NewJumpVelocity, const float& CameraMoveSpeed);

	void InitSpringArm(USpringArmComponent* SpringArm, const float& NewTargetArmLength, const FVector& NewSocketOffset);
	void SetPlayerView();
	void ZoomInOut(USpringArmComponent* NewSpringArm, const float& CameraMoveSpeed);

	UPROPERTY(VisibleAnywhere, Category = "Condition")
	EPlayerAttackCondition PlayerAttackCondition;

};

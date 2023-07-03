// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/AttackWeapon.h"
#include "GameFramework/Actor.h"
#include "ClientRocketPunch.generated.h"

/**
 * 
 */
UCLASS()
class PULLANDPUSH_API AClientRocketPunch : public AActor
{
	GENERATED_BODY()

public:
	AClientRocketPunch();

protected:
	virtual void BeginPlay() override;

#pragma region DEFAULT
	/** Default */
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URPMovementComponent> RPMovementComponent;

public:
	FORCEINLINE AActor* GetCasterActor() { if (CasterActor.IsValid()) return CasterActor.Get();	return nullptr; }
	FORCEINLINE class URPMovementComponent* GetRPMovementComponent() const { return RPMovementComponent; }

	UFUNCTION()
	void SetCollisionSimulatePhysics(bool Val);

protected:
	UPROPERTY()
	TWeakObjectPtr<AActor> CasterActor;
#pragma endregion

#pragma region LAUNCH
	/** Launch */
public:
	void ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const bool InForceVisibility, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale);

protected:
	UFUNCTION(Server, Reliable)
	void ServerReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const bool InForceVisibility, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale);

	UPROPERTY(Transient)
	float ForceAlpha;

	uint8 bIsPush : 1;
	const FName CollisionName = TEXT("RocketPunch");
#pragma endregion

#pragma region VISIBILITY
/** Visibility */
public:
	void SetMeshVisibility(bool InVisibility);

protected:
	UFUNCTION()
	void OnRep_ChangeMeshVisibility();

	UFUNCTION()
	void OnRep_ChangeMesh();

	UFUNCTION()
	void OnRep_ChangeScale();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeMeshVisibility)
	bool bStaticMeshVisibility;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeMesh)
	TWeakObjectPtr<class UStaticMesh> CurrentMesh;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeScale)
	FVector CurrentScale;

	// StaticMesh of RP
	UPROPERTY(EditDefaultsOnly, Category = "Punch | Mesh")
	TObjectPtr<class UStaticMesh> PushMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Punch | Mesh")
	TObjectPtr<class UStaticMesh> PullMesh;

#pragma endregion

public:
	UFUNCTION()
	void SetForceReturn();
};

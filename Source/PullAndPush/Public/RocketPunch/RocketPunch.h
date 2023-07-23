// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PullAndPush.h"
#include "Interface/RocketPunchHandler.h"
#include "GameData/CharacterStat.h"
#include "GameFramework/Actor.h"
#include "RocketPunch.generated.h"

DECLARE_DELEGATE_OneParam(FRocketPunchOutOfUse, const bool)

UCLASS()
class PULLANDPUSH_API ARocketPunch : public AActor, public IRocketPunchHandler
{
	GENERATED_BODY()

public:
	ARocketPunch();

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URPCollisionComponent> RPCollisionComponent;

public:
	FORCEINLINE AActor* GetCasterActor() { if (CasterActor.IsValid()) return CasterActor.Get();	return nullptr; }
	FORCEINLINE class URPMovementComponent* GetRPMovementComponent() const { return RPMovementComponent; }
	FORCEINLINE class URPCollisionComponent* GetRPCollisionComponent() const { return RPCollisionComponent; }

	virtual void SetCollisionSimulatePhysics(bool Val) override;

	FRocketPunchOutOfUse OutOfUse;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> CasterActor;

	void IsOutOfUse(const bool& Val);

	UPROPERTY(EditDefaultsOnly, Category = "RocketPunch")
	uint8 bIsServerRP : 1;
#pragma endregion

#pragma region LAUNCH
	/** Launch */
public:
	virtual void ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, FItemEnhancedStat InPassiveStat);

	UFUNCTION()
	void SetForceReturn();

protected:
	UFUNCTION(Server, Reliable)
	void ServerReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, FItemEnhancedStat InPassiveStat);

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY()
	FItemEnhancedStat CurrentPassiveStat;

	UPROPERTY(Transient)
	float ForceAlpha;

	uint8 bIsPush : 1;
	const FName CollisionName = TEXT("RocketPunch");
	const FName NoCollisionName = TEXT("NoCollision");

#pragma endregion

#pragma region VISIBILITY
/** Visibility */
public:
	virtual void SetMeshVisibility(bool InVisibility) override;
	virtual void SetMeshChange(bool IsPush);

protected:
	UFUNCTION()
	void OnRep_ChangeScale();

	UFUNCTION()
	void OnRep_ChangeMeshVisibility();

	UFUNCTION()
	void OnRep_ChangeMesh();

	UPROPERTY()
	FVector CurrentScale;

	UPROPERTY()
	bool bStaticMeshVisibility;

	UPROPERTY()
	TObjectPtr<class UStaticMesh> CurrentMesh;

	// StaticMesh of RP
	UPROPERTY(EditDefaultsOnly, Category = "Punch | Mesh")
	TObjectPtr<class UStaticMesh> PushMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Punch | Mesh")
	TObjectPtr<class UStaticMesh> PullMesh;

#pragma endregion
};

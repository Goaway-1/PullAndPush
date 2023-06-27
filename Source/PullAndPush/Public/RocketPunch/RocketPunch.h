#pragma once

#include "PullAndPush.h"
#include "Interface/AttackWeapon.h"
#include "GameFramework/Actor.h"
#include "RocketPunch.generated.h"


DECLARE_DELEGATE_OneParam(FRocketPunchOutOfUse, const bool)

UCLASS()
class PULLANDPUSH_API ARocketPunch : public AActor
{
	GENERATED_BODY()
	
public:	
	ARocketPunch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

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
	FORCEINLINE AActor* GetCasterActor() {	if (CasterActor.IsValid()) return CasterActor.Get();	return nullptr;	}
	FORCEINLINE class URPMovementComponent* GetRPMovementComponent() const {return RPMovementComponent;}
	FORCEINLINE class URPCollisionComponent* GetRPCollisionComponent() const {return RPCollisionComponent;}

	UFUNCTION()
	void SetCollisionSimulatePhysics(bool Val);

	FRocketPunchOutOfUse OutOfUse;
private:
	void IsOutOfUse(const bool& Val);

	UPROPERTY()
	TWeakObjectPtr<AActor> CasterActor;

/** Launch */
public:
	void ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale);

private:
	UFUNCTION(Server, Reliable)
	void ServerReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, const float& DeltaSpeed, const float& DeltaRange, const float& DeltaScale);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(Transient)
	float ForceAlpha;

	uint8 bIsPush : 1;
	const FName CollisionName = TEXT("RocketPunch");

/** Visibility */
public:
	void SetMeshVisibility(bool InVisibility);

private:
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
};

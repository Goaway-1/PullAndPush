#pragma once

#include "PullAndPush.h"
#include "RocketPunch/ClientRocketPunch.h"
#include "RocketPunch.generated.h"


DECLARE_DELEGATE_OneParam(FRocketPunchOutOfUse, const bool)
DECLARE_DELEGATE(FRocketPunchCallBack)


UCLASS()
class PULLANDPUSH_API ARocketPunch : public AClientRocketPunch
{
	GENERATED_BODY()
	
public:	
	ARocketPunch();

protected:
	virtual void BeginPlay() override;

#pragma region DEFAULT
/** Default */
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URPCollisionComponent> RPCollisionComponent;
	
public:
	FORCEINLINE class URPCollisionComponent* GetRPCollisionComponent() const {return RPCollisionComponent;}

	FRocketPunchOutOfUse OutOfUse;
private:
	void IsOutOfUse(const bool& Val);

#pragma endregion

/** Launch */
private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	FRocketPunchCallBack OnRocketPunchCallBack;

	UFUNCTION()
	void SetRocketPunchCallBack(bool InValue /*Nothing*/);

public:
	virtual void SetMeshVisibility(bool InVisibility) override;

	virtual void SetMeshChange(bool IsPush) override;

private:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeMeshVisibility)
	bool bStaticMeshVisibility;

	UFUNCTION()
	void OnRep_ChangeMeshVisibility();

	UFUNCTION()
	void OnRep_ChangeMesh();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ChangeMesh)
	TObjectPtr<class UStaticMesh> CurrentMesh;
};

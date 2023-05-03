#pragma once

#include "PullAndPush.h"
#include "Interface/AttackWeapon.h"
#include "GameFramework/Actor.h"
#include "RocketPunch.generated.h"


DECLARE_DELEGATE_OneParam(FRocketPunchOutOfUse, const bool&)

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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URPMovementComponent> RPMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URPCollisionComponent> RPCollisionComponent;

	void ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot, const float& AlphaSpeed, const float& AlphaRange, const float& AlphaSize);
	void IsOutOfUse(const bool& Val);
	AActor* GetCasterActor();

	FORCEINLINE class URPMovementComponent* GetRPMovementComponent() const {return RPMovementComponent;}
	FORCEINLINE class URPCollisionComponent* GetRPCollisionComponent() const {return RPCollisionComponent;}

	// For Log
	// @TODO : 추후 색상이 아닌 메시로 변경해야 함.
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	TObjectPtr<class UMaterial> PushMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<class UMaterial> PullMaterial;

	UFUNCTION()
	void SetCollisionSimulatePhysics(bool Val);

	FRocketPunchOutOfUse OutOfUse;

private:
	UPROPERTY()
	TObjectPtr<AActor> CasterActor;

	UPROPERTY(Transient)
	float ForceAlpha;

	uint8 bIsPush : 1;
	const FName CollisionName = TEXT("RocketPunch");

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};

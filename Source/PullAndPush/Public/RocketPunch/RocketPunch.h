#pragma once

#include "CoreMinimal.h"
#include "Interface/AttackWeapon.h"
#include "GameFramework/Actor.h"
#include "RocketPunch.generated.h"

UCLASS()
class PULLANDPUSH_API ARocketPunch : public AActor, public IAttackWeapon
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

	virtual void ReadyToLaunch(const float& InForceAlpha, AActor* InCasterActor, const bool IsPush, const FVector& InVec, const FRotator& InRot) override;
	virtual void IsOutOfUse(const bool& Val) override;
	virtual AActor* GetCasterActor() override;

	FORCEINLINE URPMovementComponent* GetRPMovementComponent() const {return RPMovementComponent;}
	FORCEINLINE URPCollisionComponent* GetRPCollisionComponent() const {return RPCollisionComponent;}

	// For Log
	// @TODO : 추후 색상이 아닌 메시로 변경해야 함.
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	TObjectPtr<class UMaterial> PushMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<class UMaterial> PullMaterial;

	UFUNCTION()
	void SetCollisionSimulatePhysics(bool Val);

private:
	TObjectPtr<AActor> CasterActor;
	uint8 bIsPush : 1;
	float ForceAlpha;
	const FName CollisionName = TEXT("RocketPunch");

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};

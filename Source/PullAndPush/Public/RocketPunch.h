#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketPunch.generated.h"

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

	void ReadyToLaunch(const float& Force, AActor* InOwnerPlayerActor, const bool IsPush, const FVector& InVec, const FRotator& InRot);

	FORCEINLINE URPMovementComponent* GetRPMovementComponent() const {return RPMovementComponent;}

	// For Log
	// TODO : 추후 색상이 아닌 메시로 변경해야 함.
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	TObjectPtr<class UMaterial> PushMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<class UMaterial> PullMaterial;

private:
	TObjectPtr<AActor> OwnerPlayerActor;
	uint8 bIsPush : 1;
	const FName CollisionName = TEXT("RocketPunch");

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};

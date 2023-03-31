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
public:
	void ReadyToLaunch(const float& Force, AActor* InOwnerPlayerActor);

	FORCEINLINE URPMovementComponent* GetRPMovementComponent() const {return RPMovementComponent;}
};

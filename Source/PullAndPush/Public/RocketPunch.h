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

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URPMovementComponent> RPMovementComponent;
public:
	void ReadyToLaunch(const float& Force);
	void SetOwner(class AActor* OwnActor);

	FORCEINLINE const AActor* GetOwnerActor() {return OwnerActor;}
private:
	UPROPERTY()
	TObjectPtr<class AActor> OwnerActor;

};

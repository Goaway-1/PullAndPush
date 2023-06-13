#pragma once

#include "CoreMinimal.h"
#include "Gimmick/ObstacleMover.h"
#include "InteractiveObstacleMover.generated.h"

UCLASS()
class PULLANDPUSH_API AInteractiveObstacleMover : public AObstacleMover
{
	GENERATED_BODY()

public:
	AInteractiveObstacleMover();

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void CheckHit();

	/** Get Timeline's Current Time */
	float GetCurrentTime();

protected:
	/** Hit Box */
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> HitVector;

	UPROPERTY(EditAnywhere, Category="Gimmick | Hit")
	FVector BoxCollisionExtent;

	UPROPERTY(EditAnywhere, Category = "Gimmick | Hit")
	float ForceStrength;

	/** Hit only for the time want */
	UPROPERTY(EditDefaultsOnly, Category = "Gimmick | Hit")
	uint8 bLimitedTimeHit:1;

	UPROPERTY(EditDefaultsOnly, Category = "Gimmick | Hit", meta = (EditCondition = "bLimitedTimeHit"))
	float MinHitTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gimmick | Hit", meta = (EditCondition = "bLimitedTimeHit"))
	float MaxHitTime;
};

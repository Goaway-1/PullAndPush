#include "Gimmick/InteractiveObstacleMover.h"
#include "Components/CapsuleComponent.h"
#include "Interface/CharacterInterActionHandler.h"

AInteractiveObstacleMover::AInteractiveObstacleMover()
	:
	ForceStrength(2000.f), bLimitedTimeHit(0), MinHitTime(0.f), MaxHitTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 60.f;

	HitVector = CreateDefaultSubobject<USceneComponent>(TEXT("HitVector"));
	HitVector->SetupAttachment(StaticMeshComp);
}
void AInteractiveObstacleMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckHit();
}
float AInteractiveObstacleMover::GetCurrentTime()
{
	float CurrentTimelineTime = (LocationSlideCurve != nullptr) ? LocationTimelineComp->GetPlaybackPosition() : RotationTimelineComp->GetPlaybackPosition();
	return CurrentTimelineTime;
}
void AInteractiveObstacleMover::CheckHit()
{
	// If the Hit event occurs only for the desired time
	if (bLimitedTimeHit)
	{
		float CurrentTimelineTime = GetCurrentTime();
		if (CurrentTimelineTime < MinHitTime || CurrentTimelineTime > MaxHitTime) return;
	}

	// Find objects within the sphere
	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Gimmick), false, this);
	Params.AddIgnoredActor(this);

	// Check Hited
	const FVector OriginPos = HitVector->GetComponentLocation();
	const FRotator OriginRot = HitVector->GetComponentRotation();
	const FQuat& OriginQuat = StaticMeshComp->GetComponentRotation().Quaternion();
	bool HitDetected = GetWorld()->OverlapMultiByObjectType(Overlaps, OriginPos, OriginQuat, CollisionObjectQueryParams, FCollisionShape::MakeBox(BoxCollisionExtent), Params);
	if (HitDetected) 
	{
		TArray<UPrimitiveComponent*, TInlineAllocator<1>> AffectedComponents;
		AffectedComponents.Reserve(Overlaps.Num());

		for (FOverlapResult& OverlapResult : Overlaps)
		{
			if (UPrimitiveComponent* PrimitiveComponent = OverlapResult.Component.Get())
			{
				AffectedComponents.AddUnique(PrimitiveComponent);
			}
		}

		for (UPrimitiveComponent* PrimitiveComponent : AffectedComponents)
		{
			// see if this is a target for a movement component
			AActor* ComponentOwner = PrimitiveComponent->GetOwner();
			if (ComponentOwner)
			{
				TScriptInterface<class ICharacterInterActionHandler> ActionHandler = ComponentOwner;
				if (ActionHandler.GetInterface()) {
					FVector ImpulseDirection = OriginRot.Vector() * ForceStrength;
					ActionHandler->ApplyObstacleImpulse(ImpulseDirection);
				}
			}
		}
	}

	// For log
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugBox(GetWorld(), OriginPos, BoxCollisionExtent, OriginQuat, DrawColor, false, 0.3f);
}
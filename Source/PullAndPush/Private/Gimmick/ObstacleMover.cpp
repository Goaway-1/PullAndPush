#include "Gimmick/ObstacleMover.h"
#include "Net/UnrealNetwork.h"

AObstacleMover::AObstacleMover()
	:
	MovementStartTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false); 
	NetUpdateFrequency = 60.f;

	DefaultSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	LocationTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("LocationTimelineComp"));
	RotationTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotationTimelineComp"));

	SetRootComponent(DefaultSceneComp);
	StaticMeshComp->SetupAttachment(GetRootComponent());
	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetCollisionProfileName(MeshCollisionName);
	StaticMeshComp->SetIsReplicated(true);
}
void AObstacleMover::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetInitTimeline();
}
void AObstacleMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InterpolateLocation(DeltaTime);
	InterpolateRotation(DeltaTime);
}
void AObstacleMover::SetInitTimeline()
{
	if (!LocationTimelineComp || !RotationTimelineComp) return;

	// Set Location & Rotation Slide
	if (LocationSlideCurve)
	{
		SlideLocationInterpFunction.BindDynamic(this, &AObstacleMover::ServerUpdateLocation);
		LocationTimelineComp->AddInterpVector(LocationSlideCurve, SlideLocationInterpFunction);
		LocationTimelineComp->SetLooping(true);
	}
	if (RotationSlideCurve)
	{
		SlideRotationInterpFunction.BindDynamic(this, &AObstacleMover::ServerUpdateRotation);
		RotationTimelineComp->AddInterpVector(RotationSlideCurve, SlideRotationInterpFunction);
		RotationTimelineComp->SetLooping(true);
	}

	// Set Movement Time
	if (GetWorld()->IsServer())
	{
		if (MovementStartTime < 0.1f)
		{
			StartMovement();
		}
		else
		{
			FTimerHandle TimelineHanlder;
			GetWorld()->GetTimerManager().SetTimer(TimelineHanlder, this, &AObstacleMover::StartMovement, MovementStartTime, false);
		}
	}
}
void AObstacleMover::InterpolateLocation(float DeltaTime)
{
	// interpolation of moving the actor to the predicted position
	if (LocationSlideCurve != nullptr)
	{
		FVector NewLocation = StaticMeshComp->GetRelativeLocation();
		NewLocation = FMath::VInterpTo(NewLocation, PredictedLocation, DeltaTime, InterpolationSpeed);
		StaticMeshComp->SetRelativeLocation(NewLocation);
	}
}
void AObstacleMover::InterpolateRotation(float DeltaTime)
{
	if (RotationSlideCurve != nullptr)
	{
		FQuat CurrentRotationQuat = StaticMeshComp->GetRelativeRotation().Quaternion();
		FQuat TargetRotationQuat = PredictedRotation.Quaternion();

		FQuat NewRotationQuat = FQuat::Slerp(CurrentRotationQuat, TargetRotationQuat, DeltaTime * InterpolationSpeed);
		FRotator NewRotation = NewRotationQuat.Rotator();

		StaticMeshComp->SetRelativeRotation(NewRotation);
	}
}
void AObstacleMover::ServerUpdateLocation_Implementation(const FVector NewLocation)
{
	PredictedLocation = NewLocation;
}
void AObstacleMover::ServerUpdateRotation_Implementation(const FVector NewRotator)
{
	PredictedRotation = FRotator(NewRotator.X, NewRotator.Y, NewRotator.Z);
}
void AObstacleMover::StartMovement()
{
	if(LocationSlideCurve != nullptr) LocationTimelineComp->Play();
	if(RotationSlideCurve != nullptr) RotationTimelineComp->Play();
}
void AObstacleMover::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AObstacleMover, PredictedLocation, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AObstacleMover, PredictedRotation, COND_SkipOwner);
}
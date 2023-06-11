#include "Gimmick/SliderGimmick.h"
#include "Net/UnrealNetwork.h"

ASliderGimmick::ASliderGimmick()
	:
	MovementStartTime(0.1f)
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
	StaticMeshComp->SetCollisionProfileName(CollisionName);
	StaticMeshComp->SetIsReplicated(true);						
}
void ASliderGimmick::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!LocationTimelineComp || !RotationTimelineComp) return;

	// Set Location & Rotation Slide
	if (LocationSlideCurve)
	{
		SlideLocationInterpFunction.BindDynamic(this, &ASliderGimmick::ServerUpdateLocation);
		LocationTimelineComp->AddInterpVector(LocationSlideCurve, SlideLocationInterpFunction);
		LocationTimelineComp->SetLooping(true);
	}
	if (RotationSlideCurve)
	{
		SlideRotationInterpFunction.BindDynamic(this, &ASliderGimmick::ServerUpdateRotation);
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
			GetWorld()->GetTimerManager().SetTimer(TimelineHanlder, this, &ASliderGimmick::StartMovement, MovementStartTime, false);
		}
	}
}
void ASliderGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InterpolateLocation(DeltaTime);
	InterpolateRotator(DeltaTime);
}
void ASliderGimmick::InterpolateLocation(float DeltaTime)
{
	// interpolation of moving the actor to the predicted position
	if (LocationSlideCurve != nullptr)
	{
		FVector NewLocation = StaticMeshComp->GetRelativeLocation();
		NewLocation = FMath::VInterpTo(NewLocation, PredictedLocation, DeltaTime, InterpolationSpeed);
		StaticMeshComp->SetRelativeLocation(NewLocation);
	}
}
void ASliderGimmick::InterpolateRotator(float DeltaTime)
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
void ASliderGimmick::ServerUpdateLocation_Implementation(const FVector NewLocation)
{
	PredictedLocation = NewLocation;
}
void ASliderGimmick::ServerUpdateRotation_Implementation(const FVector NewRotator)
{
	PredictedRotation = FRotator(NewRotator.X, NewRotator.Y, NewRotator.Z);
}
void ASliderGimmick::StartMovement()
{
	if(LocationSlideCurve != nullptr) LocationTimelineComp->Play();
	if(RotationSlideCurve != nullptr) RotationTimelineComp->Play();
}
void ASliderGimmick::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASliderGimmick, PredictedLocation, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ASliderGimmick, PredictedRotation, COND_SkipOwner);
}
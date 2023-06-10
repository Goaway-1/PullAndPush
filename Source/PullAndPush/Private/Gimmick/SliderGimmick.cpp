#include "Gimmick/SliderGimmick.h"
#include "Net/UnrealNetwork.h"

ASliderGimmick::ASliderGimmick()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false); 
	NetUpdateFrequency = 60.f;

	DefaultSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	TimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));

	SetRootComponent(DefaultSceneComp);
	StaticMeshComp->SetupAttachment(GetRootComponent());
	StaticMeshComp->SetGenerateOverlapEvents(false);
	StaticMeshComp->SetCollisionProfileName(CollisionName);
	StaticMeshComp->SetIsReplicated(true);						
}
void ASliderGimmick::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (TimelineComp && SlideCurve)
	{
		SlideInterpFunction.BindDynamic(this, &ASliderGimmick::ServerUpdateLocation);
		TimelineComp->AddInterpVector(SlideCurve, SlideInterpFunction);
		TimelineComp->SetLooping(true);

		// Only Move in Server
		if (GetWorld()->IsServer())
		{
			TimelineComp->Play();
		}
	}
	else
	{
		PPLOG(Warning, TEXT("Gimmick's SlideCurve is null"));
	}
}
void ASliderGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InterpolateLocation(DeltaTime);
}
void ASliderGimmick::InterpolateLocation(float DeltaTime)
{
	// interpolation of moving the actor to the predicted position
	FVector NewLocation = StaticMeshComp->GetRelativeLocation();
	NewLocation = FMath::VInterpTo(StaticMeshComp->GetRelativeLocation(), PredictedLocation, DeltaTime, InterpolationSpeed);

	StaticMeshComp->SetRelativeLocation(NewLocation);
}
void ASliderGimmick::ServerUpdateLocation_Implementation(const FVector NewLocation)
{
	PredictedLocation = NewLocation;
}
void ASliderGimmick::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASliderGimmick, PredictedLocation, COND_SkipOwner);
}

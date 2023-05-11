#include "Character/AimingComponent.h"
#include "Curves/CurveFloat.h"

UAimingComponent::UAimingComponent()
{
	ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));
	ZoomInterpFunction.BindUFunction(GetOwner(), FName("UpdateSpringArmLength"));		
}
void UAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	InitZoomTimeLine();
}
void UAimingComponent::InitZoomTimeLine()
{
	if (ZoomTimeline && ZoomCurve)
	{
		ZoomTimeline->AddInterpFloat(ZoomCurve, ZoomInterpFunction);
		ZoomTimeline->SetLooping(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlayerableCharacter] ZoomCurve is not exist"));
	}
}
void UAimingComponent::ZoomInOut(const bool IsZoom)
{
	if (IsZoom)
	{
		ZoomTimeline->Play();
		ZoomTimeline->SetPlayRate(1.f);
	}
	else
	{
		ZoomTimeline->Reverse();
		ZoomTimeline->SetPlayRate(2.f);
	}
}

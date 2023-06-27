#include "Item/DeployableItem/VelocityInhibitorDeployableItem.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interface/CharacterStatHandler.h"

AVelocityInhibitorDeployableItem::AVelocityInhibitorDeployableItem()
{
	EventCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("EventCollisionComp"));
	EventCollisionComp->SetBoxExtent(FVector(200.f,200.f,80.f));
	EventCollisionComp->SetupAttachment(GetRootComponent());
	EventCollisionComp->SetSimulatePhysics(false);
	EventCollisionComp->SetGenerateOverlapEvents(false);

	// Only when out of region will the effect disappear
	CharacterStatModifier.Stat = ECharacterStat::Slow;
	CharacterStatModifier.ChangeDuration = 100.f;
	bIsAutoActive = false;
}
void AVelocityInhibitorDeployableItem::BeginPlay()
{
	Super::BeginPlay();

	EventCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AVelocityInhibitorDeployableItem::AddOverlapActors);
	EventCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AVelocityInhibitorDeployableItem::DeleteOverlapActors);
}
void AVelocityInhibitorDeployableItem::ActiveDeployableItem()
{
	Super::ActiveDeployableItem();

	CollisionComp->SetSimulatePhysics(false);
	EventCollisionComp->SetGenerateOverlapEvents(true);
	ProjectileMovementComponent->Velocity = FVector::Zero();

	// Effect
	ensure(Effect);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, GetActorLocation(), FRotator::ZeroRotator, FVector(0.5f));
}
void AVelocityInhibitorDeployableItem::AddOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TScriptInterface<ICharacterStatHandler> CharacterHandler = OtherActor;
	if (CharacterHandler.GetInterface())
	{
		CharacterHandler->EnableStatFlag(CharacterStatModifier.Stat, CharacterStatModifier.ChangeDuration);
	}
}
void AVelocityInhibitorDeployableItem::DeleteOverlapActors(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TScriptInterface<ICharacterStatHandler> CharacterHandler = OtherActor;
	if (CharacterHandler.GetInterface())
	{
		CharacterHandler->DisableStatFlag(CharacterStatModifier.Stat);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCollisionComponent.h"

URPCollisionComponent::URPCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// TODO : �ʱ�ȭ ���ε�
	OnReset.BindUObject(this, &URPCollisionComponent::ResetOverlapActors);
}
void URPCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
void URPCollisionComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, const bool IsPush)
{
	// TODO : �浹 �� 1ȸ�� �����ϰ�, Movment�� ������ ����...
	// ���� ������ �������� ���.
	
	if (OverlapActors.Find(OtherActor)) return;

	OverlapActors.Add(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("[URPCollisionComponent] Overrlap Type is '%s', Name is '%s'"), *OtherComponent->GetCollisionProfileName().ToString(), *OtherActor->GetName());
}

void URPCollisionComponent::ResetOverlapActors()
{
	OverlapActors.Reset();
}

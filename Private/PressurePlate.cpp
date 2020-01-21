// Fill out your copyright notice in the Description page of Project Settings.

#include "PressurePlate.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

APressurePlate::APressurePlate()
	:
	PlateMeshActiveHeight(-12.0f),
	PlateMeshInactiveHeight(8.0f)
{
	OuterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterMesh"));
	if (OuterMesh)
	{
		RootComponent = OuterMesh;
		OuterMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	if (PlateMesh)
	{
		PlateMesh->SetupAttachment(RootComponent);
		PlateMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		PlateMesh->SetMobility(EComponentMobility::Movable);
	}

	if (GetTrigger())
	{
		GetTrigger()->SetupAttachment(PlateMesh);
	}

}

void APressurePlate::NotifyActorBeginOverlap(AActor * OtherActor)
{
	ATriggerVolumeSwitch::NotifyActorBeginOverlap(OtherActor);

}

void APressurePlate::NotifyActorEndOverlap(AActor * OtherActor)
{
	ATriggerVolumeSwitch::NotifyActorEndOverlap(OtherActor);

}

void APressurePlate::Interact(AActor * Interactor)
{
	ATriggerVolumeSwitch::Interact(Interactor);

	if (nullptr != PlateMesh)
	{
		FVector PlateMeshDestination = PlateMesh->RelativeLocation;
		PlateMeshDestination.Z = !IsActive() ? PlateMeshActiveHeight : PlateMeshInactiveHeight;

		PlateMesh->SetRelativeLocation(PlateMeshDestination);
	}
}

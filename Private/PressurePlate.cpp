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

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	if (Trigger)
	{
		Trigger->SetupAttachment(PlateMesh);
		Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void APressurePlate::NotifyActorBeginOverlap(AActor * OtherActor)
{
	AActor::NotifyActorBeginOverlap(OtherActor);

	Interact_Implementation(OtherActor);
}

void APressurePlate::NotifyActorEndOverlap(AActor * OtherActor)
{
	AActor::NotifyActorEndOverlap(OtherActor);

	Interact_Implementation(OtherActor);
}

void APressurePlate::Interact_Implementation(AActor * Interactor)
{
	ASwitch::Interact_Implementation(Interactor);

	if (nullptr != PlateMesh)
	{
		FVector PlateMeshDestination = PlateMesh->RelativeLocation;
		PlateMeshDestination.Z = !IsActive() ? PlateMeshActiveHeight : PlateMeshInactiveHeight;

		PlateMesh->SetRelativeLocation(PlateMeshDestination);
	}
}

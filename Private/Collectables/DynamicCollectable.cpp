// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectables/DynamicCollectable.h"
#include "Components/InterpToMovementComponent.h"
#include "Components/StaticMeshComponent.h"

ADynamicCollectable::ADynamicCollectable()
	:
	YawRotationRate(3.0f)
{
	check(GetMesh())
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Initialise the interp comp
	static FName const InterpCompName = TEXT("InterpToMovementComponent");
	InterpComp = CreateDefaultSubobject<UInterpToMovementComponent>(InterpCompName);
	check(InterpComp);
}

void ADynamicCollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// TODO: Profile to check whether constructing a rotator on Tick is expensive.
	// The answer? probably.
	AddActorWorldRotation(FRotator(0.0f, YawRotationRate, 0.0f));
}

void ADynamicCollectable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	OnCollected(OtherActor);
}
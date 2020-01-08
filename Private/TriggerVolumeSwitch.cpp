// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerVolumeSwitch.h"
#include "Components/BoxComponent.h"

ATriggerVolumeSwitch::ATriggerVolumeSwitch()
	:
	bEndOverlapCausesInteraction(false)
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	if( nullptr != Trigger )
	{
		RootComponent = Trigger;
		Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// Only overlaps, no collision
		Trigger->bMultiBodyOverlap = true;	// Multiple actors can overlap with this trigger
	}
}

void ATriggerVolumeSwitch::NotifyActorBeginOverlap(AActor * OtherActor)
{
	AActor::NotifyActorBeginOverlap(OtherActor);

	Interact_Implementation(OtherActor);
}

void ATriggerVolumeSwitch::NotifyActorEndOverlap(AActor * OtherActor)
{
	AActor::NotifyActorEndOverlap(OtherActor);
	
	if (bEndOverlapCausesInteraction)
	{
		Interact_Implementation(OtherActor);
	}
}

void ATriggerVolumeSwitch::Interact_Implementation(AActor * Interactor)
{
	ASwitch::Interact_Implementation(Interactor);

}

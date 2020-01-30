// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectables/OverlappableDynamicCollectable.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AOverlappableDynamicCollectable::AOverlappableDynamicCollectable()
{
	static FName const TriggerName = TEXT("Trigger");
	
	// Initialise and check the trigger is valid
	Trigger = CreateDefaultSubobject<USphereComponent>(TriggerName);
	check(Trigger);
	// Set the trigger as the root comp
	RootComponent = Trigger;
	Trigger->SetGenerateOverlapEvents(true); // The trigger can be overlapped

	// After checking its validity, attach the mesh to the root comp. 
	check(GetMesh()); 
	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetGenerateOverlapEvents(false); // The mesh cannot be overlapped
}

void AOverlappableDynamicCollectable::OnCollected(AActor* Collector)
{
	// TODO: OnCollected should return a bool, so I can avoid another function call
	// Call up the hierarchy so this collectable can be destroyed
	ADynamicCollectable::OnCollected(Collector);
	if (HasBeenCollected())
	{
		// Disable the trigger's collision
		Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

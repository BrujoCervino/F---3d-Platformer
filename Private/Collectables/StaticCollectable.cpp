// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectables/StaticCollectable.h"
#include "Components/StaticMeshComponent.h"

AStaticCollectable::AStaticCollectable()
{
	check(GetMesh());
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AStaticCollectable::Interact(AActor* Interactor)
{
	if (!HasBeenCollected())
	{
		OnCollected(Interactor);
	}

	IInteractable::Execute_ReceiveInteract(this, Interactor);
}

void AStaticCollectable::OnCollected(AActor* Collector)
{
	// Now call up the hierarchy so this collectable can be destroyed
	ACollectable::OnCollected(Collector);
	if (HasBeenCollected())
	{
		// Disable the mesh's collision
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectables/Collectable.h"
#include "Interactable.h"
#include "StaticCollectable.generated.h"

// TODO: Rename this to interactablecollectable

/**
 * The base class for plant and note
 * A collectable which does not move and is directly interacted with by the IINteractable implementors
 */
UCLASS()
class F_API AStaticCollectable
	: public ACollectable, public IInteractable
{
	GENERATED_BODY()

public:

	AStaticCollectable();

	virtual void Interact(AActor* Interactor) override;

	virtual void OnCollected(AActor* Collector) override;
};

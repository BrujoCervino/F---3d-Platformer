// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Door.generated.h"

// A door can be opened (and maybe closed(?) (TODO)) by the player.
UCLASS()
class F_API ADoor 
	: public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//~ Begin Interface IInteractable
	virtual void Interact(AActor* Interactor) override;
	//~ End Interface IInteractable
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ASwitch::ASwitch()
	:
	ActorsToActivate(),
	bExecuteOnce(false),
	bIsActive(false),
	ActivatedSound(nullptr),
	DeactivatedSound(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitch::Interact_Implementation(AActor * Interactor)
{
	for (AActor* const It : ActorsToActivate)
	{
		// Interact with each valid actor, passing in this switch as the instigator
		if (nullptr != It)
		{
			IInteractable::Execute_Interact(It, this);
		}
	}

	// Play a sound to feed back to the player whether this switch turned on or off
	USoundCue* const SoundToPlay = bIsActive ? DeactivatedSound : ActivatedSound;
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());

	// Invert this switch's activity state: if on, turn off (and vice versa)
	bIsActive = !bIsActive;
}


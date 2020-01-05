// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Switch.h"
#include "TriggerVolumeSwitch.generated.h"

class UBoxComponent;

/**
 * A trigger volume switch triggers interactables, when overlapped by the player.
 */
UCLASS()
class F_API ATriggerVolumeSwitch : public ASwitch
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATriggerVolumeSwitch();

	// Returns Trigger
	inline UBoxComponent* GetTrigger() const { return Trigger; }

	// Event when this actor overlaps another actor, for example a player walking into a trigger.
	// For events when objects have a blocking collision, for example a player hitting a wall, see 'Hit' events.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Event when an actor no longer overlaps another actor, and they have separated.
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	//~ Begin IInteractable Interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	//~ End IInteractable Interface

private:

	// The trigger whose overlap activates or deactivates the interactables
	UPROPERTY(VisibleAnywhere, Category="Interaction|Components", BlueprintReadOnly, meta=(AllowPrivateAccess="True") )
	UBoxComponent* Trigger;

	// Whether leaving the trigger volume triggers interaction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	uint32 bEndOverlapCausesInteraction : 1;
};

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

protected:

	// Whether leaving the trigger volume triggers interaction.
	// Most useful when dealing with a pressure plate which must have an actor triggering it at all times:
	// E.G. a pressure plate which must have a player or a weight always keeping it active.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	uint32 bEndOverlapCausesInteraction : 1;

private:

	// The trigger whose overlap activates or deactivates the interactables
	UPROPERTY(VisibleAnywhere, Category="Interaction|Components", BlueprintReadOnly, meta=(AllowPrivateAccess="true") )
	UBoxComponent* Trigger;

};

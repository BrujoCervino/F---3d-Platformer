// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerVolumeSwitch.h"
#include "PressurePlate.generated.h"

/** 
 * A pressure plate is a switch which triggers interactables, when stood upon by the player.
 */
UCLASS()
class F_API APressurePlate : public ATriggerVolumeSwitch
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APressurePlate();

	// Event when this actor overlaps another actor, for example a player walking into a trigger.
	// For events when objects have a blocking collision, for example a player hitting a wall, see 'Hit' events.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Event when an actor no longer overlaps another actor, and they have separated.
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	//~ Begin IInteractable Interface
	virtual void Interact(AActor* Interactor) override;
	//~ End IInteractable Interface

	// The plate on which the player stands to trigger this switch.
	UStaticMeshComponent* GetPlateMesh() { return PlateMesh; }

	// The mesh of this switch which does not move.
	UStaticMeshComponent* GetOuterMesh() { return OuterMesh; }

private:

	// The 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateMesh;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* OuterMesh;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float PlateMeshActiveHeight;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float PlateMeshInactiveHeight;

};

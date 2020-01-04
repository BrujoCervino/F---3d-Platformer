// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Switch.h"
#include "PressurePlate.generated.h"

class UBoxComponent;

/** 
 * A pressure plate is a switch which triggers interactables, when stood upon by the player.
 */
UCLASS()
class F_API APressurePlate : public ASwitch
{
	GENERATED_BODY()
	
public:

	APressurePlate();

	// Event when this actor overlaps another actor, for example a player walking into a trigger.
	// For events when objects have a blocking collision, for example a player hitting a wall, see 'Hit' events.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Event when an actor no longer overlaps another actor, and they have separated.
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	//~ Begin IInteractable Interface
	virtual void Interact_Implementation(AActor* Interactor) override;
	//~ End IInteractable Interface

	// 
	UBoxComponent* GetTrigger() { return Trigger; }

	// 
	UStaticMeshComponent* GetPlateMesh() { return PlateMesh; }

	// 
	UStaticMeshComponent* GetOuterMesh() { return OuterMesh; }

private:

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Trigger;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateMesh;

	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* OuterMesh;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float PlateMeshActiveHeight;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float PlateMeshInactiveHeight;

};

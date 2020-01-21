// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Switch.generated.h"

// #Features to implement:
// Timed: after a certain time, the switch returns to its original state

class USoundCue;

// A switch activates or deactivates multiple actors.
// For example, a pressure plate which opens doors
UCLASS()
class F_API ASwitch 
	: public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//~ Begin IInteractable Interface
	virtual void Interact(AActor* Interactor) override;

	//virtual void Interact_Proper(AActor* Interactor) override {}
	//~ End IInteractable Interface

	// Getter for ActorsToActivate
	inline TArray<AActor*> GetActorsToActivate() const { return ActorsToActivate; }

	// Getter for bIsActive
	inline bool IsActive() { return bIsActive; }

protected:

	// Whether this switch can only be used once
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Interaction", meta = (BlueprintProtected = "true"))
	uint32 bExecuteOnce : 1;

	// Whether this switch can only be used once
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (BlueprintProtected = "true"))
	uint32 bHasExecuted : 1;

	// Whether this switch is turned on or off.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (BlueprintProtected = "true"))
	uint32 bIsActive : 1;

private:

	// The actors with which this switch will interact
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> ActorsToActivate;

	// Sound to play when the switch is turned on
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	USoundCue* ActivatedSound;

	// Sound to play when the switch is turned off
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	USoundCue* DeactivatedSound;
};

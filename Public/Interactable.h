// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

// Allows implementors to communicate with one another, knowing who communicated.
// Example: the character interacts with a door, which tests whether they have a key, then reacts accordingly
class F_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	// Be interacted by another actor.
	// Param: AActor* Interactor: The instigator of the interaction.
	virtual void Interact(AActor* Interactor);

	// Called to let Blueprint scripts add logic to interactions
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="Interact"))
	void ReceiveInteract(AActor* Interactor);

	// Returns the position (in worldspace) of where the interactable widget should appear.
	// This is projected from screen to worldspace 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetInteractableWidgetSocketLocation() const;
};
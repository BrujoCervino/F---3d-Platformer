// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlatformerTypes.h"
#include "Collector.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCollector : public UInterface
{
	GENERATED_BODY()
};

/**
 * Called by collectables, to pass their type to a collector.
 * E.g. a collectable can call this on a player to tell the player what type of collectable they just picked up.
 * This interface exists so I can eventually allow enemies to pick up collectables and run off with them, 
 *	until killed by the player.
 */
class F_API ICollector
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	virtual void Collect(const TEnumAsByte<ECollectableTypes::Type> TypeCollected);

	// Allows Blueprints to 
	// Note: TEnumAsByte cannot be used on the Blueprint implementation of this function, 
	//	as it conflicts with the return type in the generated header.
	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName="Collect"))
	void ReceiveCollect(const ECollectableTypes::Type TypeCollected);
};

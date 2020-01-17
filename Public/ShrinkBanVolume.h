// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "ShrinkBanVolume.generated.h"

// TODO: I need some way to visually represent the shrink ban volume: maybe particles fly around in it?
// TODO: This should also be able to ban (maxification/sizing up) instead of minification. 
//	I need to include an option for this an an exposed toggle to Blueprints

/**
 * 
 */
UCLASS()
class F_API AShrinkBanVolume : public AVolume
{
	GENERATED_BODY()
	
public:

	// Event when this actor overlaps another actor, for example a player walking into a trigger.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Event when an actor no longer overlaps another actor, and they have separated.
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};

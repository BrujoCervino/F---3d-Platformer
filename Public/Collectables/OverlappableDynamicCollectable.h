// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectables/DynamicCollectable.h"
#include "OverlappableDynamicCollectable.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class F_API AOverlappableDynamicCollectable : public ADynamicCollectable
{
	GENERATED_BODY()

public:

	AOverlappableDynamicCollectable();

	// Returns trigger
	inline USphereComponent* GetTrigger() const { return Trigger; }

	// 
	virtual void OnCollected(AActor* Collector) override;

private:

	// The trigger whose overlap causes this collectable to be collected
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectable", meta = (AllowPrivateAccess = "true"))
	USphereComponent* Trigger;

};

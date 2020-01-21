// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// TODO: Move this to "Collectables/..."

#include "CoreMinimal.h"
#include "Collectables/DynamicCollectable.h"
#include "LevelEndCrystal.generated.h"

UCLASS()
class F_API ALevelEndCrystal : public ADynamicCollectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelEndCrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void OnCollected(AActor* Collector);

};

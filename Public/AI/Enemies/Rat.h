// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Collector.h"
#include "Rat.generated.h"

UCLASS()
class F_API ARat 
	: public ACharacter, public ICollector
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Collect(const TEnumAsByte<ECollectableTypes::Type> TypeCollected);

	// Accessors for  CurrentlyHeldCollectable.
	inline class ACollectable* GetCurrentlyHeldCollectable() const { return CurrentlyHeldCollectable; }
	inline void SetCurrentlyHeldCollectable(ACollectable* const InCollectable) { CurrentlyHeldCollectable = InCollectable; }

private:

	// The collectable which the rat has collected. Dropped back into the world (unhidden and made collisive) when this rat is killed.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collectables", meta = (AllowPrivateAccess = "true"))
	ACollectable* CurrentlyHeldCollectable;
};

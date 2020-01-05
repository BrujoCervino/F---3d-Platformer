// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class F_API APlatformerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APlatformerPlayerController();

	// Getter for bShrinkUnlocked
	inline bool IsShrinkUnlocked() const { return bShrinkUnlocked; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Whether the player has unlocked the shrink ability yet
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	uint32 bShrinkUnlocked : 1;

};

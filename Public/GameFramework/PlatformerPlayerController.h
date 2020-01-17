// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformerPlayerController.generated.h"

// CONSIDER: I want the max to be 

/**
 * The default player controller for this game
 * 
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

	// Get the 
	inline int32 GetSkillPointsEarnt() const { return SkillPointsEarnt; }
	inline int32 GetSkillPointsSpent() const { return SkillPointsSpent; }					  
	inline int32 GetSkillPointsEarntPostDeductions() const { return SkillPointsEarnt - SkillPointsSpent; }

	// Give this player one more skill point
	UFUNCTION(BlueprintCallable)
	virtual void GrantSkillPoint();

	// Spend a specified amount of skill points 
	UFUNCTION(BlueprintCallable)
	virtual void ConsumeSkillPoints(const int32 Cost);

	// If this character has enough keys, use one.
	// Returns whether a key was used.
	UFUNCTION(BlueprintCallable)
	virtual bool UseKey();

	// Increments keys
	UFUNCTION(BlueprintCallable)
	virtual void GiveKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Load game data, via a save file
	void LoadFromSaveData();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Shrink is an ability used by the player to reach hidden areas
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// Whether the player has unlocked the shrink ability yet
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	uint32 bShrinkUnlocked : 1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Skill points, gained by completing secret missions, are used to buy/upgrade new moves (shrink et cetera)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// The number of skill points this player has earnt
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 SkillPointsEarnt;

	// The number of skill points this player has spent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 SkillPointsSpent;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Keys are used to unlock doors
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// The number of keys held by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BlueprintProtected = "true"))
	int32 Keys;
};

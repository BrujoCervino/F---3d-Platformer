// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "PlatformerTypes.generated.h"

// The types of collectable which the player can find, collect and use
// Note: each collectable can only be used/spent once
UENUM(BlueprintType)
namespace ECollectableTypes
{
	enum Type
	{
		None = 0,
		
		//~~~~~~~~~~~~~~~~~~~
		// Main collectables:
		//~~~~~~~~~~~~~~~~~~~
		
		// The most common collectable, orbs are the breadcrumb collectables of the game: 
		//	littered about the levels to lead the player.
		Orb, 
		// 7 flies can be found in each main level.
		Fly,
		// Pieces of big keys with which to unlock the next level. 
		Cell UMETA(DisplayName = "Cell (key piece)"),
		
		//~~~~~~
		// Keys:
		//~~~~~~

		// 
		SmallKey UMETA(DisplayName="Small Key", Tooltip="Multiple small keys can be found in each level: each one can be used to unlock any standard door."),
		// These keys can be used unlock
		KeyToSecretLevel UMETA(DisplayName = "Key To Secret Level"),
		
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Merchant-specific collectables: 
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// Plants can be found in normal levels, and can be traded with the plant-loving merchant for extra rewards
		Plant,

		//~~~~~~~
		// Notes:
		//~~~~~~~

		// 
		Note,

		//~~~~~~~~
		// Points:
		//~~~~~~~~

		SkillPoint
	};
}
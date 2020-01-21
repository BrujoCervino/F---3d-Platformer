// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEndCrystal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlatformerCharacter.h"
#include "PlatformerProgressionLibrary.h"

// Sets default values
ALevelEndCrystal::ALevelEndCrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CollectableType = ECollectableTypes::SkillPoint;
}

// Called when the game starts or when spawned
void ALevelEndCrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelEndCrystal::OnCollected(AActor* Collector)
{
	// Open the level the character was in before this one
	UPlatformerProgressionLibrary::RequestLevelComplete(this);

	ADynamicCollectable::OnCollected(Collector);
}


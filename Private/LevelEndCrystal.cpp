// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEndCrystal.h"
#include "Components/InterpToMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlatformerCharacter.h"
#include "PlatformerProgressionLibrary.h"

// Sets default values
ALevelEndCrystal::ALevelEndCrystal()
	:
	YawRotationRate(3.0f),
	bCollected(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Initialise the mesh
	static FName const MeshName = TEXT("Mesh");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(MeshName);
	if (Mesh)
	{
		RootComponent = Mesh;
		// The mesh acts as a trigger volume. If this proves too performance-intensive, 
		//	I'll switch to a capsule component for triggering
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  
	}

	// Initialise the interp comp
	static FName const InterpCompName = TEXT("InterpToMovementComponent");
	InterpComp = CreateDefaultSubobject<UInterpToMovementComponent>(InterpCompName);
	check(InterpComp);

}

// Called when the game starts or when spawned
void ALevelEndCrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelEndCrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.0f, YawRotationRate, 0.0f));
}

void ALevelEndCrystal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor && !bCollected)
	{
		// Disallow collecting this crystal twice 
		bCollected = true;
		
		// Give the player one skill point for having completed this secret level
		if (APlatformerCharacter* const PlayerCharacter = Cast<APlatformerCharacter, AActor>(OtherActor))
		{
			PlayerCharacter->GrantSkillPoint();
		}

		// Open the level the character was in before this one
		UPlatformerProgressionLibrary::RequestLevelComplete(this);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SecretLevelEntrance.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlatformerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASecretLevelEntrance::ASecretLevelEntrance()
	:
	LevelToOpen(),
	bLocked(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Only once, declare and initialise names for this door's components, to save compile and creation times
	static FName const SharedRootName = TEXT("SharedRoot");
	static FName const LeftDoorMeshName = TEXT("LeftDoorMesh");
	static FName const RightDoorMeshName = TEXT("RightDoorMesh");

	// Initialise the shared root
	SharedRoot = CreateDefaultSubobject<USceneComponent>(SharedRootName);
	// Ensure the shared root has been created
	check(SharedRoot);
	// Set the shared root as the main pivot of this entrance
	SetRootComponent(SharedRoot);

	// Initialise the left door mesh
	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(LeftDoorMeshName);
	// Ensure the mesh has been created
	check(LeftDoorMesh);
	// Attach the left door to the root comp
	LeftDoorMesh->SetupAttachment(RootComponent);

	// Initialise the right door mesh
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(RightDoorMeshName);
	// Ensure the mesh has been created
	check(RightDoorMesh);
	// Attach the right door to the root comp
	RightDoorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASecretLevelEntrance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecretLevelEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASecretLevelEntrance::Interact_Proper(AActor* Interactor)
{
	bool bShouldOpenLevel = false;

	// If this entrance is unlocked,
	if (!IsLocked())
	{
		// The level should be opened
		bShouldOpenLevel = true;
	}
	// Elsewise, if the interactor was the player,
	else if (APlatformerCharacter* const PlayerCharacter = Cast<APlatformerCharacter, AActor>(Interactor))
	{
		// If the player was able to use a key,
		if (PlayerCharacter->UseKey())
		{
			// Unlock this entrance
			bLocked = false;
			// The level should be opened
			bShouldOpenLevel = true;
		}
	}
	
	if (bShouldOpenLevel)
	{
		UGameplayStatics::OpenLevel(this, LevelToOpen);
	}
}


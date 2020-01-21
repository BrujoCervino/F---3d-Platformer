// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectables/Collectable.h"
#include "Components/StaticMeshComponent.h"
#include "Collector.h"

// Sets default values
ACollectable::ACollectable()
	:
	CollectableType(ECollectableTypes::None),
	bCollected(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	static FName const MeshName = TEXT("Mesh");

	// Initialise and check the mesh is valid
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(MeshName);
	check(Mesh)
		// Initialise and check the mesh is valid
		RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();

}

void ACollectable::OnCollected(AActor* Collector)
{
	if (!HasBeenCollected())
	{
		if (ICollector* const CollectorCast = Cast<ICollector, AActor>(Collector))
		{
			// Disallow this collectable from being collected multiple times.
			bCollected = true;
			// Notify the collector that it collected something.
			CollectorCast->Collect(CollectableType);

			// Make this collectable invisible and untouchable
			SetActorHiddenInGame(true);

			// Notify Blueprint scripting that this collectable was picked up.
			ICollector::Execute_ReceiveCollect(Collector, CollectableType);

			Destroy();
		}
	}
}




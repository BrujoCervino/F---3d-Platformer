// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructibles/DestructibleDoor.h"
#include "DestructibleComponent.h"

// Sets default values
ADestructibleDoor::ADestructibleDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static FName const MeshName = TEXT("Mesh");
	// Initialise the mesh, before checking its validity and making it the root/pivot component.
	Mesh = CreateDefaultSubobject<UDestructibleComponent>(MeshName);
	check(Mesh);
	RootComponent = Mesh;
}

float ADestructibleDoor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = AActor::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// TODO: Call ApplyDamage on the mesh

	return ActualDamage;
}


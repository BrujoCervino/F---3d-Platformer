// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleDoor.generated.h"

class UDestructibleComponent;

// A door which the player can destroy, by using stomp.
UCLASS()
class F_API ADestructibleDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleDoor();

	// Receive damage from another actor
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Returns mesh
	inline UDestructibleComponent* GetMesh() const { return Mesh; }

private:

	// The default mesh associated with this door
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Meshes", meta=(AllowPrivateAccess="true"))
	UDestructibleComponent* Mesh;

};

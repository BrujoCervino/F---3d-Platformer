// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectables/Collectable.h"
#include "DynamicCollectable.generated.h"

class UInterpToMovementComponent;

/**
 * 
 */
UCLASS()
class F_API ADynamicCollectable : public ACollectable
{
	GENERATED_BODY()
	
public:

	ADynamicCollectable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when this actos is overlapped
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	// Returns InterpCOmp
	inline UInterpToMovementComponent* GetInterpComp() const { return InterpComp; }

private:

	// The component responsible for oscillating this crystal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInterpToMovementComponent* InterpComp;

	// The rate (degrees) at which to rotate this mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float YawRotationRate;
};

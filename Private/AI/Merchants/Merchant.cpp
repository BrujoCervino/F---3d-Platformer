// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Merchants/Merchant.h"

// Sets default values
AMerchant::AMerchant()
	:
	bTalking(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMerchant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMerchant::Interact(AActor* Interactor)
{
	// Trigger the Blueprint version of this event
	IInteractable::Execute_ReceiveInteract(this, Interactor);
}


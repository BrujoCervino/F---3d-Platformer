// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerCharacter.h"
#include "PlatformerPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/ConfigCacheIni.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Interactable.h"

// Sets default values
APlatformerCharacter::APlatformerCharacter()
	:
	// Air Dash
	DashSpeed(200.0f),
	bCanAirDash(true),
	bCanAirDashSecondary(false),
	bHasUsedAirDashSecondary(false),
	AirDashSound(nullptr),
	// bAirDashIsCameraRelative(false) // Read this from a .ini, because it's player preference
	// Shrink
	bCanShrink(true),
	ShrinkSound(nullptr),
	SizeUpSound(nullptr),
	ShrunkScale(0.4f),
	StandardScale(1.0f),
	ShrunkTimeDilation(0.75f),
	StandardTimeDilation(1.0f),
	ShrinkCooldown(0.3f),
	ShrinkCooldownHandle(),
	// Interact
	bCanInteract(true),
	InteractionTraceLength(600.0f),
	InteractionTraceCapsuleRadius(42.0f),
	InteractionTraceCapsuleHalfHeight(96.0f),
	InteractionTraceDesiredTypes(),
	bInteractionTraceComplex(false),
	InteractionTraceActorsToIgnore(),
	bDrawInteractionTrace(false),
	InteractionSucceededCue(nullptr),
	InteractionFailedCue(nullptr),
	InteractionCooldown(0.2f),
	InteractionCooldownHandle(),
	// Stomp
	StompingGravityScale(6.0f),
	StandardGravityScale(1.5f),
	StompLandedCameraShake(UCameraShake::StaticClass())
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	if (GConfig)
	{
		// For now, ensure Game.ini contains our config variables.
		// Setting them NEEDS to be moved to another class
		GConfig->SetBool(TEXT("F.Abilities.Air Dash"), TEXT("bAirDashIsCameraRelative"), false, GGameIni);
		GConfig->Flush(false, GGameIni);

		// Load player-tweakable variables from the main game config file:
		bool bTemp = false;
		if (GConfig->GetBool(TEXT("F.Abilities.Air Dash"), TEXT("bAirDashIsCameraRelative"), bTemp, GGameIni))
		{
			bAirDashIsCameraRelative = bTemp;
		}
#if !UE_BUILD_SHIPPING
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not defined within %s"), GET_MEMBER_NAME_STRING_CHECKED(APlatformerCharacter, bAirDashIsCameraRelative), *GGameIni);
		}
#endif // !UE_BUILD_SHIPPING
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = StandardGravityScale;
	}
}

// Called when the game starts or when spawned
void APlatformerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlatformerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlatformerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Core movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlatformerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlatformerCharacter::MoveRight);

	PlayerInputComponent->BindAction("FacePlayerDirection", IE_Pressed, this, &APlatformerCharacter::FacePlayerDirection);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Abilities
	PlayerInputComponent->BindAction("Air Dash", IE_Pressed, this, &APlatformerCharacter::AirDash);
	PlayerInputComponent->BindAction("Shrink", IE_Pressed, this, &APlatformerCharacter::ToggleShrink);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlatformerCharacter::TraceForInteractables);
	PlayerInputComponent->BindAction("Stomp", IE_Pressed, this, &APlatformerCharacter::Stomp);

}

void APlatformerCharacter::MoveForward(float Value)
{
	if ((nullptr != Controller) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator YawRotation(
			0.0f,
			Controller->GetControlRotation().Yaw,
			0.0f
		);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlatformerCharacter::MoveRight(float Value)
{
	if ((nullptr != Controller) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator YawRotation(
				0.0f, 
				Controller->GetControlRotation().Yaw, 
				0.0f
			);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlatformerCharacter::FellOutOfWorld(const UDamageType & dmgType)
{
	// Don't call Super::FellOutOfWorld, as we don't want to destroy this actor
	OnFellOutOfWorld();
}

void APlatformerCharacter::Landed(const FHitResult & Hit)
{
	// Landing should feel powerful, so shake the screen
	if (APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller))
	{
		const bool bLandedFromStomping = (GetCharacterMovement()->GravityScale == StompingGravityScale);
		const float ShakeScale = bLandedFromStomping ? 1.0f : 0.3f;
		
		PC->ClientPlayCameraShake(StompLandedCameraShake, ShakeScale);
	}

	// Reenable standard air dashing
	bCanAirDash = true;
	// Disable extra air dashing, because only shrinking in mid-air enables this
	bCanAirDashSecondary = false;
	bHasUsedAirDashSecondary = false;
	// Reenable stomp
	GetCharacterMovement()->GravityScale = StandardGravityScale;
}

void APlatformerCharacter::FacePlayerDirection()
{
	// Get the mesh's right vector as a rotator
	Controller->SetControlRotation(GetMesh()->GetRightVector().ToOrientationRotator());
}

void APlatformerCharacter::AirDash()
{
	// If this character can dash and is in the air,
	if (GetCharacterMovement()->IsFalling())
	{				
		if (bCanAirDash)
		{
			// Disable standard air dashing until this character lands
			bCanAirDash = false;
		}
		else if(!bHasUsedAirDashSecondary && bCanAirDashSecondary)
		{
			// Disable extra air dashing until this character lands
			bCanAirDashSecondary = false;
			bHasUsedAirDashSecondary = true;
		}
		else
		{
			return;
		}
		
		if ((nullptr != Controller) && (DashSpeed != 0.0f))
		{
			// find out which way is forward
			const FRotator YawRotation(
				0.0f,
				Controller->GetControlRotation().Yaw,
				0.0f
			);

			// Had to use the right vector because the default mesh defaultly faces right
			const FVector Direction = 
				!bAirDashIsCameraRelative ? 
				GetMesh()->GetRightVector() : 
				FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// Air dashing cancels stomping - this enables more precise platforming
			GetCharacterMovement()->GravityScale = StandardGravityScale;
				
			// Propell the character
			LaunchCharacter(Direction*DashSpeed, false, true);

			// Play a sound to confirm to the player that the dash succeeded
			UGameplayStatics::PlaySoundAtLocation(this, AirDashSound, GetActorLocation());
		}
	}
}

void APlatformerCharacter::SetCanAirDash(const bool bNewCanAirDash)
{
	bCanAirDash = bNewCanAirDash;
}

void APlatformerCharacter::ToggleShrink()
{
	if (bCanShrink)
	{
		// Get the player controller in the correct format. 
		// (Keep it only const-pointer, because I'll add camera shake and controller vibration later)
		APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller);
		if (nullptr != PC)
		{
			// If the player has unlocked the shrink ability,
			if (PC->IsShrinkUnlocked())
			{
				const bool& bReturningToNormalSize = IsShrunk(); // Use an alias for clearer code
				
				float ScaleToApply			= ShrunkScale;
				USoundCue* SoundToPlay		= ShrinkSound;
				float TimeDilationToApply	= ShrunkTimeDilation;
				bool bAllowAirDashSecondary	= true;

				if (bReturningToNormalSize)
				{
					ScaleToApply			= StandardScale;
					SoundToPlay				= SizeUpSound;
					TimeDilationToApply		= StandardTimeDilation;
					bAllowAirDashSecondary	= false;
				}

				// Shrink or unshrink the player
				const FVector VectorisedScaleToApply = FVector::OneVector * ScaleToApply;
				SetActorScale3D(VectorisedScaleToApply);
				// Play a sound
				UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
				// Slow or speed back up time, so the resized character feels different to play:
				// A shrunk player is slower for player choice/risk versus reward:
				// (pro: "weightless" floatier jumps 
				// con: slower movement) 
				CustomTimeDilation = TimeDilationToApply;

				// Allow an extra air dash:
				// * If the player is in the air 
				// * if the player has just turned small
				if (!bHasUsedAirDashSecondary && GetCharacterMovement()->IsFalling())
				{
					bCanAirDashSecondary = bAllowAirDashSecondary;
				}
				

				// Ban shrinking
				SetCanShrink(false);

				// Put a cooldown on shrinking: reenable it after the cooldown.
				FTimerDelegate ShrinkCooldownDelegate;
				ShrinkCooldownDelegate.BindUFunction(this, FName("SetCanShrink"), true);
				
				GetWorldTimerManager().SetTimer(ShrinkCooldownHandle, ShrinkCooldownDelegate, ShrinkCooldown, false);
			
				// Call the Blueprint version of this function: this helps us to rapidly add prototypical features to this function
				OnShrinkToggled();
			}
		}		
	}
}

void APlatformerCharacter::SetCanShrink(const bool bNewCanShrink)
{
	bCanShrink = bNewCanShrink;
}

bool APlatformerCharacter::IsShrunk() const
{
	const FVector VectorisedShrunkScale = FVector::OneVector * ShrunkScale;
	const FVector& CurrentScale = GetActorScale3D();

	return VectorisedShrunkScale.Equals(CurrentScale, 0.01f);
}
	
void APlatformerCharacter::TraceForInteractables()
{
	if (bCanInteract)
	{
		// Remove this ugly line in future updates: it can be replaced with the actual camera once this character is fully ported to C++
		UCameraComponent const * const Camera = Cast<UCameraComponent, UActorComponent>( GetComponentByClass(UCameraComponent::StaticClass()) );
		if (nullptr != Camera)
		{
			const FVector& CamLoc = Camera->GetComponentLocation();
			const FVector TraceEndPosition = (Camera->GetForwardVector() * InteractionTraceLength) + CamLoc;

			FHitResult Hit = FHitResult();
			const bool IgnoreSelfWithinInteractionTrace = true;

			const EDrawDebugTrace::Type InteractionTraceDrawDebugType 
				= bDrawInteractionTrace ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None;
			
			UKismetSystemLibrary::CapsuleTraceSingleForObjects(this, CamLoc, TraceEndPosition, InteractionTraceCapsuleRadius, InteractionTraceCapsuleHalfHeight, InteractionTraceDesiredTypes, 
				bInteractionTraceComplex, InteractionTraceActorsToIgnore, InteractionTraceDrawDebugType, Hit, IgnoreSelfWithinInteractionTrace);
			// If the trace hit something,
			if (Hit.bBlockingHit)
			{
				// Make a quicker to type (into Intellisense) alias. 
				// This also:
				// * Avoids both extra getter calls
				// * Avoids using the weak pointer of FHitResult::Actor
				// * Is non-nullable
				AActor* const & InteractableActor = Hit.GetActor(); 

				if (nullptr != InteractableActor)
				{
					USoundCue* SoundToPlay = nullptr;
					// If we hit something interactable,
					if ( !InteractableActor->IsPendingKill() && InteractableActor->Implements<UInteractable>())
					{
						// Interact with InteractableActor, telling it this player instigated the interaction
						IInteractable::Execute_Interact(InteractableActor, this);
						SoundToPlay = InteractionSucceededCue;
					}
					// Elsewise, we hit something but it's not interactable:
					else
					{
						SoundToPlay = InteractionFailedCue;
					}
					
					// Play a sound to feed back the interaction's result to the player.
					UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, TraceEndPosition);
					
					// Ban interacting
					SetCanInteract(false);

					// Allow the player to interact after the cooldown has finished.
					FTimerDelegate InteractionCooldownDelegate;
					InteractionCooldownDelegate.BindUFunction(this, FName("SetCanInteract"), true);

					GetWorldTimerManager().SetTimer(InteractionCooldownHandle, InteractionCooldownDelegate, InteractionCooldown, false);

					// React to this interaction
					OnInteracted();
				}
			}
		}
	}
}

void APlatformerCharacter::SetCanInteract(const bool bNewCanInteract)
{
	bCanInteract = bNewCanInteract;
}

void APlatformerCharacter::Stomp()
{
	// If this player is airborne,
	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->GravityScale = StompingGravityScale;
	}
}


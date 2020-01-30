// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformerCharacter.h"
#include "PlatformerPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h" // SphereTraceForActors
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Misc/ConfigCacheIni.h" // Config (ini) variables
#include "Sound/SoundCue.h"
#include "TimerManager.h" // Timers
#include "Interactable.h" 
#include "PlatformerTypes.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
APlatformerCharacter::APlatformerCharacter()
	:
	// * Movement & Camera
	BaseTurnRate(45.0f),
	BaseLookUpRate(45.0f),
	//bUseOneHandedCamera(false), // Read this from a .ini, because it's player preference
	// * Damage
	bInvincible(false),
	InvincibilityDuration(1.5f),
	InvincibilityTimerHandle(),
	MaxHealth(3.0f),
	Health(MaxHealth),
	DamagedLaunchStrength(600.0f),
	DamagedColour(FColor::Red),
	DamagedCameraShake(UCameraShake::StaticClass()),
	DamagedSound(nullptr),
	DamagedVibration(nullptr),
	StandardSceneFringeIntensity(0.6f), // Maybe this should be set later, from postinitcomps: could read from the actual postprocessing
	DamagedSceneFringeIntensity(5.0f),
	SceneFringeResetDuration(0.5f),
	SceneFringeResetTimerHandle(),
	MostRecentCheckpointLocation(FVector::ZeroVector),
	CheckpointRegisteredCue(nullptr),
	CheckpointRegisteredColour(FColor::Magenta),
	DeathTransitionRadiusName(),
	DeathTransitionRadiusProgress(0.0f),
	DeathTransitionMaterialParamCollectionType(nullptr),
	DeathTransitionMaterialParamCollectionInst(nullptr),
	DeathTransitionTimerFrequency(0.1f),
	DeathTransitionTimerHandle(),
	// * Collectables
	KeyCollectedColour(FColor::Orange),
	// * Lantern
	LanternMeshSocketName(),
	LanternHandleBoneName(),
	// * Air Dash
	DashSpeed(200.0f),
	bCanAirDash(true),
	bCanAirDashSecondary(false),
	bHasUsedAirDashPrimary(false),
	bHasUsedAirDashSecondary(false),
	AirDashSound(nullptr),
	// bAirDashIsCameraRelative(false) // Read this from a .ini, because it's player preference
	// * Shrink
	bCanShrink(true),
	ShrinkToggledCameraShake(UCameraShake::StaticClass()),
	ShrinkSound(nullptr),
	SizeUpSound(nullptr),
	ShrunkScale(0.4f),
	StandardScale(1.0f),
	ShrunkTimeDilation(0.75f),
	StandardTimeDilation(1.0f),
	ShrinkCooldown(0.3f),
	ShrinkCooldownHandle(),
	// * Interact
	CurrentInteractable(nullptr),
	bCanInteract(true),
	InteractionSucceededCue(nullptr),
	InteractionFailedCue(nullptr),
	InteractionCooldown(0.2f),
	InteractionCooldownHandle(),
	bCharacterRelativeInteractionTrace(true),
	// * Stomp
	StompRadius(200.0f),
	StompDamage(2.0f),
	RadialForceTimeActive(0.5f),
	StompableClass(AActor::StaticClass()),
	RadialForceTimeActiveTimerHandle(),
	StompHitTypes(),
	StompingGravityScale(6.0f),
	StandardGravityScale(1.5f),
	StompLandedCameraShake(UCameraShake::StaticClass()),
	CurrentlyHeldItem(EItemTypes::None)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Ensure the gravity scale is the one this player uses when not shrunk
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = StandardGravityScale;
	}

	// Create a camera boom (pulls in towards the player if there is a collision)
	static FName const CameraBoomName = TEXT("CameraBoom");
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(CameraBoomName);
	if (CameraBoom)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	}

	// Create a follow camera
	static FName const FollowCameraName = TEXT("FollowCamera");
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FollowCameraName);
	if (FollowCamera)
	{
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm 
	}

	// Initialise the interaction capsule
	static FName const InteractionCapsuleName = TEXT("Interaction Capsule");
	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(InteractionCapsuleName);
	check(InteractionCapsule);
	InteractionCapsule->SetupAttachment(GetMesh());
	InteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlatformerCharacter::OnIteractionCapsuleBeginOverlap);
	InteractionCapsule->OnComponentEndOverlap.AddDynamic(this, &APlatformerCharacter::OnInteractionCapsuleEndOverlap);

	// Initialise the lantern mesh
	static FName const LanternName = TEXT("LanternMesh");
	LanternMesh = CreateDefaultSubobject<USkeletalMeshComponent>(LanternName);
	check(LanternMesh);
	LanternMesh->SetupAttachment(GetMesh(), LanternMeshSocketName); 
	//LanternMesh->Attachtocom

	// Initialise the radial force comp
	static FName const RadialForceCompName = TEXT("RadialForceComp");
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(RadialForceCompName);
	check(RadialForceComp);
	RadialForceComp->SetupAttachment(RootComponent);

	// Config variables
	if (GConfig)
	{
		// For now, ensure Game.ini contains our config variables.
		// Setting them NEEDS to be moved to another class
		GConfig->SetBool(TEXT("F.Abilities.Air Dash"), GET_MEMBER_NAME_STRING_CHECKED(APlatformerCharacter, bAirDashIsCameraRelative), false, GGameIni);
		GConfig->SetBool(TEXT("F.Abilities.Air Dash"), GET_MEMBER_NAME_STRING_CHECKED(APlatformerCharacter, bUseOneHandedCamera), false, GGameIni);
		
		GConfig->Flush(false, GGameIni);

		// Load player-tweakable variables from the main game config file:
		bool bTemp = false;
		if (GConfig->GetBool(TEXT("F.Abilities.Air Dash"), GET_MEMBER_NAME_STRING_CHECKED(APlatformerCharacter, bAirDashIsCameraRelative), bTemp, GGameIni))
		{
			bAirDashIsCameraRelative = bTemp;
		}
#if !UE_BUILD_SHIPPING
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not defined within %s"), GET_MEMBER_NAME_STRING_CHECKED(APlatformerCharacter, bAirDashIsCameraRelative), *GGameIni);
		}
#endif // !UE_BUILD_SHIPPING

		bool bTemp2 = false;
		if(GConfig->GetBool(TEXT("F.Abilities.Air Dash"), GET_MEMBER_NAME_STRING_CHECKED(APlatformerCharacter, bUseOneHandedCamera), bTemp, GGameIni))
		{ 
			bUseOneHandedCamera = bTemp2;
		}
	}

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void APlatformerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MostRecentCheckpointLocation = GetActorLocation();
	
	if (UWorld* const CurrentWorld = GetWorld())
	{
		CurrentWorld->GetParameterCollectionInstance(DeathTransitionMaterialParamCollectionType);
	}

	LanternMesh->SetAllBodiesBelowSimulatePhysics(LanternHandleBoneName, true, false);
}

// Called every frame
void APlatformerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Remove this from Tick: once enabled, it should call APlatformerCharacter::FacePlayerDirection
	// and disable camera input (event-based approach).
	if (bUseOneHandedCamera)
	{
		// Accessibility: lock the camera to the mesh's forward vector	
		FacePlayerDirection();
	}
}

// Called to bind functionality to input
void APlatformerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// * Core movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlatformerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlatformerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// * Camera
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlatformerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlatformerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("FacePlayerDirection", IE_Pressed, this, &APlatformerCharacter::FacePlayerDirection);

	// * Abilities
	PlayerInputComponent->BindAction("Air Dash", IE_Pressed, this, &APlatformerCharacter::AirDash);
	PlayerInputComponent->BindAction("Shrink", IE_Pressed, this, &APlatformerCharacter::ToggleShrink);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlatformerCharacter::TryInteract);
	PlayerInputComponent->BindAction("Stomp", IE_Pressed, this, &APlatformerCharacter::Stomp);

}

void APlatformerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlatformerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
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

void APlatformerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	// Don't call Super::FellOutOfWorld, as we don't want to destroy this actor
	Die(nullptr);

	OnFellOutOfWorld();
}

void APlatformerCharacter::Landed(const FHitResult& Hit)
{
	ACharacter::Landed(Hit);
	
	float ShakeScale = 0.3f;

	const bool bLandedFromStomping = (GetCharacterMovement()->GravityScale == StompingGravityScale);
	if (bLandedFromStomping)
	{
		LandedFromStomping(Hit);
		ShakeScale = 1.0f;
	}

	// Landing should feel powerful, so shake the screen
	if (APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller))
	{
		PC->ClientPlayCameraShake(StompLandedCameraShake, ShakeScale);
	}

	// Reenable standard air dashing
	bCanAirDash = true;
	bHasUsedAirDashPrimary = false;
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

float APlatformerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = 0.0f;
	
	// Avoids continuous damage (uses invincibility frames):
	// If this player can take damage,
	if (!bInvincible)
	{
		ActualDamage = AActor::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		
		// Temporarily make this player invincible
		bInvincible = true;

		// Set a timer which disables invincibility
		static const FName SetInvincibleFunctionName = GET_FUNCTION_NAME_CHECKED(APlatformerCharacter, SetInvincible);
		
		FTimerDelegate InvincibilityDelegate;
		InvincibilityDelegate.BindUFunction(this, SetInvincibleFunctionName, false);
		
		GetWorldTimerManager().SetTimer(InvincibilityTimerHandle, InvincibilityDelegate, InvincibilityDuration, false);

		// Apply the damage
		Health -= ActualDamage;

		// If health reaches 0 or lower,
		if (FMath::Min(Health, 0.0f) == Health)
		{
			// This player has died
			Die(DamageCauser);
		}
		else
		{
			// React to the damage the player just took:
			// Flash a different colour
			ApplyRadialColour(DamagedColour);
			
			// Launch the character 
			FVector const DamagedJumpVelocity = FVector::UpVector * DamagedLaunchStrength;
			LaunchCharacter(DamagedJumpVelocity, false, true);

			if (FollowCamera)
			{
				// Distort the scene subtly, for player feedback
				FollowCamera->PostProcessSettings.SceneFringeIntensity = DamagedSceneFringeIntensity;

				// Interpolate back to the scene's look before distortion
				GetWorldTimerManager().SetTimer(SceneFringeResetTimerHandle, this, &APlatformerCharacter::ResetSceneFringe, SceneFringeResetDuration, false);
			}
			// For feedback:
			// Play a sound
			UGameplayStatics::PlaySoundAtLocation(this, DamagedSound, GetActorLocation());

			// Play a screenshake and a vibration 
			if (APlayerController* const PC = Cast<APlayerController, AController>(Controller))
			{
				PC->ClientPlayCameraShake(DamagedCameraShake);
				PC->ClientPlayForceFeedback(DamagedVibration);
			}
		}
	}
	return ActualDamage;
}

void APlatformerCharacter::Die(AActor* Killer)
{
	OnDied(Killer);
	
	// This should be called on a timer
	Respawn();

	///////GetWorldTimerManager().SetTimer(DeathTransitionTimerHandle, this, &APlatformerCharacter::Prerespawn, DeathTransitionTimerFrequency, true);
}

void APlatformerCharacter::Prerespawn()
{
	DeathTransitionRadiusProgress += 0.1f;
	if (DeathTransitionRadiusProgress <= 1.0f)
	{
		check(DeathTransitionMaterialParamCollectionInst);
		// Set the MPC parameter
		DeathTransitionMaterialParamCollectionInst->SetScalarParameterValue
		(
			DeathTransitionRadiusName, 
			FMath::Lerp(0.0f, 1.0f, DeathTransitionRadiusProgress)
		);
	}
	else
	{
		Respawn();
		GetWorldTimerManager().ClearTimer(DeathTransitionTimerHandle);
	}
}

void APlatformerCharacter::Respawn()
{
	// Restore health to maximum
	Health = MaxHealth;

	// Place the character at the location of the last encountered checkpoint
	SetActorLocation(MostRecentCheckpointLocation);

	// Avoids the common glitch in games, where a small amount of velocity is kept by a respawned player
	// (which could be built up to speed/teleport through the level)
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}

	// Ensure the player is not shrunk
	EnsureStandardSize(false);

	// Ensure the player is not stomping
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = StandardGravityScale;
	}

	// Ensure the player is not invincible
	bInvincible = false;
}

void APlatformerCharacter::RegisterCheckpoint(AActor const * const CheckpointActor)
{
	// If the checkpoint actor was valid,
	if (CheckpointActor)
	{
		// Let its location be the last known checkpoint
		FVector const & CheckpointLoc = CheckpointActor->GetActorLocation();
		MostRecentCheckpointLocation = CheckpointLoc;

		// Play a sound, for feedback
		UGameplayStatics::PlaySoundAtLocation(this, CheckpointRegisteredCue, CheckpointLoc);
		// Change this player's colour, for feedback
		ApplyRadialColour(CheckpointRegisteredColour);
	}
}

void APlatformerCharacter::SetInvincible(const bool bNewInvincible)
{
	bInvincible = bNewInvincible;
}

void APlatformerCharacter::ResetSceneFringe()
{
	if (FollowCamera)
	{
		// Get the normalised result of the elapsed time into the duration of the timer. 
		const float TimeRemaining = GetWorldTimerManager().GetTimerElapsed(SceneFringeResetTimerHandle);
		const float& MaxTime = SceneFringeResetDuration;
		const float NormalisedTimeRemaining = TimeRemaining / MaxTime;
		// Lerp by this, into resetting the postprocessing
		FollowCamera->PostProcessSettings.SceneFringeIntensity = 
			FMath::Lerp(DamagedSceneFringeIntensity, StandardSceneFringeIntensity, NormalisedTimeRemaining);
	}
}

void APlatformerCharacter::Collect(const TEnumAsByte<ECollectableTypes::Type> TypeCollected)
{
	// Ensure Blueprints know this character collected something
	ICollector::Execute_ReceiveCollect(this, TypeCollected);

	USoundCue* CollectedSound = nullptr;
	FLinearColor CollectedColour = FColor::Transparent;
	
	switch (TypeCollected)
	{
		case(ECollectableTypes::None):
		{
			checkNoEntry();
			break;
		}
		case(ECollectableTypes::Orb):
		{
			break;	
		}
		case(ECollectableTypes::Fly):
		{
			break;
		}
		case(ECollectableTypes::Cell):
		{
			break;
		}
		case(ECollectableTypes::SmallKey):
		{
			GiveKey();
			CollectedColour = KeyCollectedColour;
			break;
		}
		case(ECollectableTypes::KeyToSecretLevel):
		{
			GiveSecretKey();
			CollectedColour = KeyCollectedColour;
			break;
		}
		case(ECollectableTypes::Plant):
		{
			break;
		}
		case(ECollectableTypes::Note):
		{
			break;
		}
		case(ECollectableTypes::SkillPoint):
		{
			GrantSkillPoint();
			break;
		}
		default:
		{
			checkNoEntry();
			break;
		}
	}

	UGameplayStatics::PlaySoundAtLocation(this, CollectedSound, GetActorLocation());
	ApplyRadialColour(CollectedColour);
}

bool APlatformerCharacter::UseKey()
{
	// If the player controller exists, try to use a key.
	bool bUsedKey = false;
	if (APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller))
	{
		bUsedKey = PC->UseKey();
	}

	return bUsedKey;
}

bool APlatformerCharacter::GiveKey()
{
	// If the player controller exists, give a key to this player.
	bool bGaveKey = false;
	if (APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller))
	{
		PC->GiveKey();
		bGaveKey = true;
	}

	ApplyRadialColour(KeyCollectedColour);

	return bGaveKey;
}

bool APlatformerCharacter::UseSecretKey()
{
	// If the player controller exists, try to use a key.
	bool bUsedKey = false;
	if (APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller))
	{
		bUsedKey = PC->UseSecretKey();
	}

	return bUsedKey;
}

bool APlatformerCharacter::GiveSecretKey()
{
	// If the player controller exists, give a key to this player.
	bool bGaveKey = false;
	if (APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller))
	{
		PC->GiveSecretKey();
		bGaveKey = true;
	}

	ApplyRadialColour(KeyCollectedColour);

	return bGaveKey;
}

bool APlatformerCharacter::GrantSkillPoint()
{
	// If the player controller is valid, grant a skill point
	bool bSuccess = false;
	if (APlatformerPlayerController* const PC = Cast<APlatformerPlayerController, AController>(Controller))
	{
		PC->GrantSkillPoint();
		bSuccess = true;
	}

	return bSuccess;
}

void APlatformerCharacter::AirDash()
{
	// If this character can dash and is in the air,
	if (GetCharacterMovement()->IsFalling())
	{
		if (!bHasUsedAirDashPrimary && bCanAirDash)
		{
			// Disable standard air dashing until this character lands
			bCanAirDash = false;
			bHasUsedAirDashPrimary = true;			
		}
		else if (!bHasUsedAirDashSecondary && bCanAirDashSecondary)
		{
			// Disable extra air dashing until this character lands
			bCanAirDashSecondary = false;
			bHasUsedAirDashSecondary = true;
		}
		else
		{
			return;
		}

		if ((nullptr != Controller) && (0.0f != DashSpeed ))
		{
			// find out which way is forward
			const FRotator YawRotation
			(
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
			LaunchCharacter(Direction * DashSpeed, false, true);

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
				const bool& bAboutToDecreaseSize = !IsShrunk(); // Use an alias for clearer code

				float ScaleToApply = StandardScale;
				USoundCue* SoundToPlay = SizeUpSound;
				float TimeDilationToApply = StandardTimeDilation;

				if (bAboutToDecreaseSize)
				{
					ScaleToApply = ShrunkScale;
					SoundToPlay = ShrinkSound;
					TimeDilationToApply = ShrunkTimeDilation;

					// Allow an extra air dash if this player:
					// * Has just turned small
					// * Has used standard air dash
					// * Has not already used secondary air dash
					// * Is in the air
					// This makes secondary air dash a technique for advanced players
					if (bHasUsedAirDashPrimary && !bHasUsedAirDashSecondary && GetCharacterMovement()->IsFalling())
					{
						bCanAirDashSecondary = true;
					}
				}

				// Shrink or unshrink the player
				const FVector VectorisedScaleToApply = FVector(ScaleToApply);
				SetActorScale3D(VectorisedScaleToApply);
				// Play a sound
				UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
				// Play a camera shake
				PC->ClientPlayCameraShake(ShrinkToggledCameraShake);
				// Slow or speed back up time, so the resized character feels different to play:
				// A shrunk player is slower for player choice/risk versus reward:
				// (pro: "weightless" floatier jumps 
				// con: slower movement) 
				CustomTimeDilation = TimeDilationToApply;

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
	const FVector VectorisedShrunkScale = FVector(ShrunkScale);
	const FVector& CurrentScale = GetActorScale3D();

	return VectorisedShrunkScale.Equals(CurrentScale, 0.01f);
}

void APlatformerCharacter::EnsureStandardSize(const bool bShouldPlaySound /*= true*/)
{
	CustomTimeDilation = StandardTimeDilation;
	SetActorScale3D(FVector(StandardScale));

	if (bShouldPlaySound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SizeUpSound, GetActorLocation());
	}
}

void APlatformerCharacter::OnIteractionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && this != OtherActor && OtherActor->Implements<UInteractable>())
	{
		SetCanInteract(true);
		CurrentInteractable = OtherActor;
	}
}

void APlatformerCharacter::OnInteractionCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetCanInteract(false);
	//CurrentInteractable = nullptr;
}

void APlatformerCharacter::TryInteract()
{
	if(bCanInteract && CurrentInteractable)
	{
		// Avoid a useless assignment to nullptr, by assuming nothing interactable was found
		USoundCue* SoundToPlay = InteractionFailedCue;

		CurrentInteractable->ClearPendingKill();
		// Interact with InteractableActor, telling it this player instigated the interaction
		if (IInteractable* const InteractableActorCast = Cast<IInteractable, AActor>(CurrentInteractable))
		{
			InteractableActorCast->Interact(this);
		}

		SoundToPlay = InteractionSucceededCue;

		// Play a sound to feed back the interaction's result to the player.
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());

		// Ban interacting
		SetCanInteract(false);

		// Allow the player to interact after the cooldown has finished.
		static const FName SetCanInteractFunctionName = GET_FUNCTION_NAME_CHECKED(APlatformerCharacter, SetCanInteract);
		
		FTimerDelegate InteractionCooldownDelegate;
		InteractionCooldownDelegate.BindUFunction(this, SetCanInteractFunctionName, true);

		GetWorldTimerManager().SetTimer(InteractionCooldownHandle, InteractionCooldownDelegate, InteractionCooldown, false);

		// React to this interaction, via Blueprint scripting
		OnInteracted();
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
		OnStomped();
	}
}

void APlatformerCharacter::LandedFromStomping(const FHitResult& Hit)
{
	// TODO: spawn a decal of cracked ground
	
	// Get where the hit happened.
	const FVector& HitLoc = GetActorLocation();
	// Ignore this actor only
	TArray<AActor*> ActorsToIgnore = { this }; 
	TArray<AActor*> FoundActors; // Leave empty for now: filled by reference
	// Check for actors within StompRadius
	// TODO: If using this implementation of SphereOverlapActors proves too costly, use UWorld::MultiOverlapForActors. 
	// However, for what we want here, only a line or two of code would be saved.
	UKismetSystemLibrary::SphereOverlapActors(this, HitLoc, StompRadius, StompHitTypes, *StompableClass, ActorsToIgnore, FoundActors);
	
	// Add force to each found primitive component within radius.
	
	// TODO: Remove the radial force comp, if too costly, and write its functionality here.
	RadialForceComp->Activate();
	RadialForceComp->FireImpulse();
	
	// Stop applying force, after the specified time.
	static FName const RadialForceDeactivateFunctionName = GET_FUNCTION_NAME_CHECKED(URadialForceComponent, Deactivate);
	FTimerDelegate RadialForceTimeOutDelegate;
	RadialForceTimeOutDelegate.BindUFunction(RadialForceComp, RadialForceDeactivateFunctionName, false);
	GetWorldTimerManager().SetTimer(RadialForceTimeActiveTimerHandle,RadialForceTimeOutDelegate, RadialForceTimeActive, false); // Deactivate the radial force comp, after a timer.
	
	// Apply damage to each found component
	const FDamageEvent DmgEv = FDamageEvent();
	for (AActor* const It : FoundActors)
	{
		if (It)
		{
			It->TakeDamage(StompDamage, DmgEv, Controller, this);
		}
	}

	// Notify Blueprints that we stomped
	OnLandedFromStomping(Hit);
}


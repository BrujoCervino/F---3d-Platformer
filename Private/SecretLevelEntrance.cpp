#include "SecretLevelEntrance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlatformerCharacter.h"

// Sets default values
ASecretLevelEntrance::ASecretLevelEntrance()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASecretLevelEntrance::Interact(AActor* Interactor)
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
		// If the player was able to use a secret key,
		if (PlayerCharacter->UseSecretKey())
		{
			// Unlock this entrance
			SetLocked(false);
			// The level should be opened
			bShouldOpenLevel = true;
		}
	}

	if (bShouldOpenLevel)
	{
		UGameplayStatics::OpenLevel(this, GetLevelToOpen());
	}
}

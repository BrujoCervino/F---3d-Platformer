// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PlatformerGameModeBase.h"
#include "ThirdParty/discord-files/core.h"

// TODO: Set this up properly:
// Create a Discord core object pointer
discord::Core* coreDiscordObject{};

APlatformerGameModeBase::APlatformerGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

//TODO: Move this to OnDestroyed, or just make core a UE4 smart pointer
APlatformerGameModeBase::~APlatformerGameModeBase()
{
    
    
    delete coreDiscordObject;
    coreDiscordObject = nullptr;
}

void APlatformerGameModeBase::BeginPlay()
{
    Super::BeginPlay();

   auto result = discord::Core::Create(670248635103182849, DiscordCreateFlags_NoRequireDiscord, &coreDiscordObject);
   discord::Activity activity = {}; 
   activity.SetState("Forshrunken");

#if !UE_BUILD_SHIPPING	
    static const char* const Details = "Beta Testing";
#else
   static const char* const Details = "Developing";
#endif // UE_BUILD_SHIPPING	
   activity.SetDetails(Details);
   coreDiscordObject->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
       });   
   coreDiscordObject->ActivityManager().RegisterCommand("F.exe");
}

void APlatformerGameModeBase::Tick(float DeltaTime)
{
   ::coreDiscordObject->RunCallbacks();
}

void APlatformerGameModeBase::CompleteGame()
{

}

// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class F : ModuleRules
{
	public F(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            // Default :
            "Core", "CoreUObject", "Engine", "InputCore",
            // Extras :
			"ApexDestruction", "SteelSeriesGameSense"
		});

		// Add Discord SDK
		PublicIncludePaths.Add("D:/_proj/UE4/Fun/F/Source/F/ThirdParty/discord-files");
		// Add Discord library
		PublicAdditionalLibraries.Add("D:/_proj/UE4/Fun/F/Binaries/Win64/discord_game_sdk.dll.lib");
		// Ensure Discord dll is added to the cooked project
		RuntimeDependencies.Add("D:/_proj/UE4/Fun/F/Binaries/Win64/discord_game_sdk.dll");

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

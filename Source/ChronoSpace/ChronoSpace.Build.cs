// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class ChronoSpace : ModuleRules
{
	public ChronoSpace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "ChronoSpace" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "UMG", "GameplayTasks",
			"Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

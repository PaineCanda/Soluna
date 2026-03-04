// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Soluna : ModuleRules
{
	public Soluna(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTasks", "GameplayTags"} );

		PublicIncludePaths.AddRange(new string[] {
			"Soluna",
			"Soluna/Variant_Platforming",
			"Soluna/Variant_Combat",
			"Soluna/Variant_Combat/AI",
			"Soluna/Variant_SideScrolling",
			"Soluna/Variant_SideScrolling/Gameplay",
			"Soluna/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

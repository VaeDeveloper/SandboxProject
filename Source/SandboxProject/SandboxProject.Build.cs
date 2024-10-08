// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SandboxProject : ModuleRules
{
	public SandboxProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"SlateCore",
            "LocalizationCommandletExecution",
            "GameplayTags",
			"DelegateModule",
			"ThreadsModule"
		});
	}
}

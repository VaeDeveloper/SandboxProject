using UnrealBuildTool;
 
public class VisualizerModule : ModuleRules
{
	public VisualizerModule(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"UnrealEd", 
			"DelegateModule",
			"SandboxProject"
		});

        PrivateDependencyModuleNames.AddRange(new string[] { "EditorFramework" });

        PublicIncludePaths.AddRange(new string[] {"VisualizerModule/Public"});
		PrivateIncludePaths.AddRange(new string[] {"VisualizerModule/Private"});
	}
}
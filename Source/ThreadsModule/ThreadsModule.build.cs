using UnrealBuildTool;
 
public class ThreadsModule : ModuleRules
{
	public ThreadsModule(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"ThreadsModule/Public" });
		PrivateIncludePaths.AddRange(new string[] {"ThreadsModule/Private"});
	}
}
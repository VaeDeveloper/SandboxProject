using UnrealBuildTool;
 
public class AsyncActions : ModuleRules
{
	public AsyncActions(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"AsyncActions/Public"});
		PrivateIncludePaths.AddRange(new string[] {"AsyncActions/Private"});
	}
}
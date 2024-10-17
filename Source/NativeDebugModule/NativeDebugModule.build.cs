using UnrealBuildTool;
 
public class NativeDebugModule : ModuleRules
{
	public NativeDebugModule(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"NativeDebugModule/Public"});
		PrivateIncludePaths.AddRange(new string[] {"NativeDebugModule/Private"});
	}
}
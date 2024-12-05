using UnrealBuildTool;
 
public class TagsModule : ModuleRules
{
	public TagsModule(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameplayTags" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags" });
 
		PublicIncludePaths.AddRange(new string[] {"TagsModule/Public"});
		PrivateIncludePaths.AddRange(new string[] {"TagsModule/Private"});
	}
}
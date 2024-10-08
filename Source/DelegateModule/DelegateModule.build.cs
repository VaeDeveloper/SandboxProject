using UnrealBuildTool;
 
public class DelegateModule : ModuleRules
{
	public DelegateModule(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Slate", "SlateCore", "EnhancedInput", "LocalizationCommandletExecution", "GameplayTags", "InputCore"});
 
		PublicIncludePaths.AddRange(new string[] {"DelegateModule/Public"});
		PrivateIncludePaths.AddRange(new string[] {"DelegateModule/Private"});
	}
}
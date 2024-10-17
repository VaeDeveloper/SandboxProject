using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class NativeDebugModule : ModuleRules
{
    public NativeDebugModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(new string[]
            {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "SlateCore",
            "MeshDescription",
            "StaticMeshDescription" ,
            "Kismet"
            });
        }

        PublicDependencyModuleNames.AddRange(new string[]
        {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "Slate",
                    "SlateCore",
                    "MeshDescription",
                    "StaticMeshDescription"
        });

        PublicIncludePaths.AddRange(new string[] { "NativeDebugModule/Public" });
        PrivateIncludePaths.AddRange(new string[] { "NativeDebugModule/Private" });
    }
}
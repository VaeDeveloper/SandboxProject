// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SandboxProjectTarget : TargetRules
{
	public SandboxProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange( new string[] { "NativeDebugModule" } );
		ExtraModuleNames.AddRange( new string[] { "AsyncActions" } );
		ExtraModuleNames.AddRange( new string[] { "ThreadsModule" } );
		ExtraModuleNames.AddRange( new string[] { "VisualizerModule" } );
		ExtraModuleNames.AddRange( new string[] { "DelegateModule" } );
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("SandboxProject");
	}
}

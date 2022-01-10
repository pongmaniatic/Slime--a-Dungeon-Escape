// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SlimePlatformer : ModuleRules
{
	public SlimePlatformer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara" });
	}
}

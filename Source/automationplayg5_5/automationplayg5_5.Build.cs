// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class automationplayg5_5 : ModuleRules
{
	public automationplayg5_5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

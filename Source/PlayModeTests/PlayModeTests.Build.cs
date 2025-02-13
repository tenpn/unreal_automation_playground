using UnrealBuildTool;

public class PlayModeTests : ModuleRules
{
	public PlayModeTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new [] { "CQTest", "automationplayg5_5", "DataValidation" });
	}
}

using UnrealBuildTool;

public class EditModeTests : ModuleRules
{
	public EditModeTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new [] { "CQTest", "UnrealEd", "automationplayg5_5", "DataValidation" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EchoBlade : ModuleRules
{
	public EchoBlade(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "FASAttribute","GameplayTags", "MotionTrajectory" });
	}
}

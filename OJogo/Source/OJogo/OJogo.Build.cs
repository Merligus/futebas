// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OJogo : ModuleRules
{
	public OJogo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "Slate", "SlateCore", "LevelSequence", "MovieScene" });
	}
}

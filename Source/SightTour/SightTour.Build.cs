// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SightTour : ModuleRules
{
    public SightTour(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "SightTour",
                "SightTour/Pickup",
                "SightTour/StateTree",
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "HeadMountedDisplay",
                "EnhancedInput",
                "NavigationSystem",
                "DeveloperSettings",
                "AIModule",
                "StateTreeModule",
                "GameplayStateTreeModule",
                "GameplayTasks",
                "GameplayTags",
            });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Slate",
                "SlateCore",
                "PropertyPath",
                "StructUtils",
                "UMG",
                "CommonUI",
                "CommonInput",
                "GameSettings",
                "CommonGame",
                "CommonUser",
                "MoviePlayer",
            }
        );
    }
}

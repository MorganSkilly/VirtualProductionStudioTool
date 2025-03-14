// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class nDisplayMeshTool : ModuleRules
{
	public nDisplayMeshTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "ProceduralMeshComponent",
                "AssetRegistry",
                "StaticMeshDescription",
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "ProceduralMeshComponent",
                "AssetRegistry",
                "StaticMeshDescription",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
            {
                "Core",
                "ProceduralMeshComponent",
                "AssetRegistry",
                "StaticMeshDescription",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
                "SlateCore",
                "ProceduralMeshComponent",
                "AssetRegistry",
                "StaticMeshDescription",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

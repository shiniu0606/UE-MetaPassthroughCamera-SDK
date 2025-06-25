// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MetaPassthrough : ModuleRules
{
	public MetaPassthrough(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core", "CoreUObject", "Engine", "InputCore", "AndroidPermission"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        PrivateDependencyModuleNames.Add("Launch");

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "MetaPassthrough_UPL.xml"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Android", "android -platform-sdk-72.aar"));
            //string SourceAarPath = Path.Combine(ModuleDirectory, "Android", "android-platform-sdk-72.aar");

        }
    }
}

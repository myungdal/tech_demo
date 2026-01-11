using System.IO;
using UnrealBuildTool;

public class MapEditor : ModuleRules
{
    public MapEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bUseRTTI = true;

        //PublicDefinitions.Add("_UE5");
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = Path.GetFullPath(Path.Combine(ModuleDirectory, "../MapEditor/Private/pch_map_editor.h"));

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Client",
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Json",
                "JsonUtilities",
                "SlateCore",
                "Slate",
                "EditorStyle",
                "UnrealEd",
                "LevelEditor",
                "ClientUtil",
                "Common",
                "Packet",
                "MmoSync",
                "SharedLib"
            });

        PrivateDependencyModuleNames.AddRange(new string[] {
            });

        PrivateIncludePaths.AddRange(new string[] {
                Path.GetFullPath(Path.Combine(ModuleDirectory, "../MapEditor/Private/")),
                Path.GetFullPath(Path.Combine(ModuleDirectory, "../Client/Private/")),
                Path.GetFullPath(Path.Combine(ModuleDirectory, "../ClientUtil/Public/")),
                Path.GetFullPath(Path.Combine(ModuleDirectory, "../Common/Public/")),
                Path.GetFullPath(Path.Combine(ModuleDirectory, "../Packet/Public/")),
                Path.GetFullPath(Path.Combine(ModuleDirectory, "../MmoSync/Public/")),
                Path.GetFullPath(Path.Combine(ModuleDirectory, "../SharedLib/Public/"))
            });
    }
}

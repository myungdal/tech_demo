using System.IO;
using UnrealBuildTool;

public class SharedLib : ModuleRules
{
    public SharedLib(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bUseRTTI = true;

        PCHUsage = PCHUsageMode.NoPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Json",
            "JsonUtilities",
            "Common",
            "Packet",
            "MmoSync"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Common/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Packet/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../MmoSync/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/"))
        });

        PrivateIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/SharedLib"))
        });
    }
}

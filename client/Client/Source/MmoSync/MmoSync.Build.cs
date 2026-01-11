using System.IO;
using UnrealBuildTool;

public class MmoSync : ModuleRules
{
    public MmoSync(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bUseRTTI = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/MmoSync/PCH/pch_mmoSync.h"));

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Json",
            "JsonUtilities",
            "Common",
            "Packet"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Common/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Packet/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/MmoSync/PCH"))
        });

        PrivateIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/MmoSync"))
        });
    }
}

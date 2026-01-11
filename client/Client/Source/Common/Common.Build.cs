using System.IO;
using UnrealBuildTool;

public class Common : ModuleRules
{
    public Common(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bUseRTTI = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Common/PCH/pch_common.h"));

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Json",
            "JsonUtilities"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
        });

        PublicIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Common/PCH")),
        });

        PrivateIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/Common"))
        });
    }
}

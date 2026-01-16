using System.IO;
using UnrealBuildTool;

public class Packet : ModuleRules
{
    public Packet(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bUseRTTI = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Packet/PCH/pch_packet.h"));

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Json",
            "JsonUtilities",
            "Common"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
        });

        PublicIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Common/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Packet/PCH/"))
        });

        PrivateIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/Packet"))
        });
    }
}

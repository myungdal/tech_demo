using System.IO;
using UnrealBuildTool;

public class ClientUtil : ModuleRules
{
    public ClientUtil(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bUseRTTI = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/ClientUtil/PCH/pch_client_util.h"));

        PublicDependencyModuleNames.AddRange(
            new string[]
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

        PrivateDependencyModuleNames.AddRange(new string[] {            
        });

        PublicIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Common/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Packet/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../MmoSync/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/ClientUtil/PCH"))
        });

        PrivateIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/ClientUtil"))
        });
    }
}

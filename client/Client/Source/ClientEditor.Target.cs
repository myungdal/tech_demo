using UnrealBuildTool;
using System.Collections.Generic;

public class ClientEditorTarget : TargetRules
{
    public ClientEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor; // Specifies this is an Editor target
        DefaultBuildSettings = BuildSettingsVersion.V5; // Build settings for UE 5.5
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        CppStandard = CppStandardVersion.Cpp20;

        ExtraModuleNames.AddRange(new string[] {
            "Client",
            "MapEditor"
        });
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class Client : ModuleRules
{
    public Client(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        bUseRTTI = true;

        CppCompileWarningSettings.UnusedValueWarningLevel = WarningLevel.Default;

        //PublicDefinitions.Add("_UE5"); __UNREAL__를 사용함
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;        
        PrivatePCHHeaderFile = Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/pch_client_private.h"));

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Json",
                "JsonUtilities",
                "Slate",
                "SlateCore",
                "UMG",
                "Sockets",
                "Networking",
                "Paper2D",
                "ClientUtil",
                "MmoSync",
                "SharedLib",
                "Common",
                "Packet"
            });

        PrivateDependencyModuleNames.AddRange(new string[] {            
        });

        PublicIncludePaths.AddRange(new string[] {
        });

        PrivateIncludePaths.AddRange(new string[] {
            Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../ClientUtil/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Common/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../Packet/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../MmoSync/Public/")),
            Path.GetFullPath(Path.Combine(ModuleDirectory, "../SharedLib/Public/"))
        });


        //if ((Target.Platform == UnrealTargetPlatform.Win64))
        //{
        //}
        //else if (Target.Platform == UnrealTargetPlatform.Android)
        //{
        //}
        //else if (Target.Platform == UnrealTargetPlatform.IOS)
        //{
        //}

        if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("AndroidRuntimeSettings");
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.Add("IOSRuntimeSettings");

            //PublicFrameworks.AddRange(new string[] {
            //	"CoreTelephony", // IOS 국가 코드 얻기 용도.
            //});
        }

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}

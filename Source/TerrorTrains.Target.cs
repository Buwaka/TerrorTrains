// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TerrorTrainsTarget : TargetRules
{
	public TerrorTrainsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("TerrorTrains");
        //bCompileAgainstEngine = true;
        //bCompileAgainstCoreUObject = true;
        bForceDebugInfo = true;
        bDisableDebugInfo = false;
        bDisableDebugInfoForGeneratedCode = false;
        bOmitPCDebugInfoInDevelopment = false;
        //bAdaptiveUnityDisablesPCH = false;
        //bUseIncrementalLinking = false;
        //bUseSharedPCHs = false;
        bUseUnityBuild = false;
        // bDebugBuildsActuallyUseDebugCRT = true;
        //bUseInlining = false;

    }
}

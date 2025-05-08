// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpinningBombTarget : TargetRules
{
	public SpinningBombTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            bUsesSteam = true;
        }

        ExtraModuleNames.AddRange( new string[] { "SpinningBomb" } );

        if (Target.Configuration == UnrealTargetConfiguration.Shipping)
        {
            bUseLoggingInShipping = false;  // 출시 빌드에서 로깅 비활성화
            bUseChecksInShipping = false;   // 출시 빌드에서 검사 비활성화
        }
        else
        {
            bUseLoggingInShipping = true;   // 개발 중 로깅 활성화
        }
    }
}

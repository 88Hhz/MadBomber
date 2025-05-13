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
            bUseLoggingInShipping = false;  // ��� ���忡�� �α� ��Ȱ��ȭ
            bUseChecksInShipping = false;   // ��� ���忡�� �˻� ��Ȱ��ȭ
        }
        else
        {
            bUseLoggingInShipping = true;   // ���� �� �α� Ȱ��ȭ
        }
    }
}

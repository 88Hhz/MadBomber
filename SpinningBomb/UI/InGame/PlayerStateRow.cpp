// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/PlayerStateRow.h"

#include "BombPlayerState.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"



void UPlayerStateRow::SetPlayerStateBlock(APlayerState* PlayerState)
{
	ABombPlayerState* State = Cast<ABombPlayerState>(PlayerState);
	if (State)
	{
		PlayerIndex->SetText(FText::FromString(FString::FromInt(PlayerState->GetPlayerId())));
		PlayerName->SetText(FText::FromString(State->GetPlayerName()));
		//PlayerHpBar->SetPercent(State->GetPlayerHp() / 20);
		PlayerCoin->SetText(FText::FromString(FString::FromInt(20)));
	}

}

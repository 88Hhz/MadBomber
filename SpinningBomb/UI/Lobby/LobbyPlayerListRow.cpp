// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyPlayerListRow.h"

#include "BombPlayerState.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"




//void ULobbyPlayerListRow::SetLobbyPlayerList(APlayerState* PlayerState)
//{
//	ABombPlayerState* State = Cast<ABombPlayerState>(PlayerState);
//	if (State)
//	{
//		if (State->GetPlayerAvatarTexture() != nullptr)
//		{
//			PlayerAvatar->SetBrushFromTexture(State->GetPlayerAvatarTexture());
//		}
//		PlayerName->SetText(FText::FromString(State->GetPlayerName()));
//	}
//}

void ULobbyPlayerListRow::SetPlayerName(const FString& Name)
{
	PlayerName->SetText(FText::FromString(Name));

}

void ULobbyPlayerListRow::ReadyForGame()
{
	bIsReady = !bIsReady;
	if (bIsReady)
	{
		ReadyState->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		ReadyState->SetColorAndOpacity(FLinearColor::White);
	}
}

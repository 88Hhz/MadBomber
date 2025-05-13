// Fill out your copyright notice in the Description page of Project Settings.


#include "BombPlayerState.h"
#include "Net/UnrealNetwork.h"	//DOREPLIFETIME

#include "GameMode/LobbyGameMode.h"
#include "PlayerController/LobbyPlayerController.h"
#include "Actor/Character/LobbyCharacter.h"

#include "BombGameInstance.h"

//#include "Engine/Texture2D.h"

#include "Kismet/GameplayStatics.h"


ABombPlayerState::ABombPlayerState()
{

}

void ABombPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (APlayerController* PC = GetPlayerController())
	{
		ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);
		if (LobbyPC != nullptr)
		{
			SetPlayerName(LobbyPC->GetUserName());
			//SetUniqueId(LobbyPC->GetUniqueNetId());
			//SteamAvatarTexture = LobbyPC->GetSteamAvatarTexture();
		}
		
	}

	if (GetUniqueId().IsValid())
	{
		FString UniquenetID = GetUniqueId()->ToString();
		UE_LOG(LogTemp, Warning, TEXT("PlayerState: UniqueId : %s"), *UniquenetID);
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerStateName : %s"), *GetPlayerName());
}


void ABombPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void ABombPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABombPlayerState, bIsReady);
	DOREPLIFETIME(ABombPlayerState, MatchIndex);

}


// LobbyGameMode���� Client�� ���� ���� ����ȭ �� ���� ���� ��ư ����
void ABombPlayerState::SetReady(bool NewReady)
{
	if (HasAuthority())
	{
		bIsReady = NewReady;

		if (ALobbyGameMode* GM = GetWorld()->GetAuthGameMode<ALobbyGameMode>())
		{
			GM->CheckAllPlayerReadyState();
		}
	}
	//bIsReady�� Replicated�� Ŭ���̾�Ʈ������.. ����ȣ��
	OnRep_Ready();
}


void ABombPlayerState::SetMatchIndex(int32 NewIndex)
{
	if (HasAuthority())
	{
		MatchIndex = NewIndex;
	}
}

void ABombPlayerState::OnRep_Ready()
{
	if (APawn* P = GetPawn())
	{
		if (ALobbyCharacter* LC = Cast<ALobbyCharacter>(P))
		{
			// Character�� WidgetComponent ������Ʈ
			LC->UpdateReadyState();
		}
	}
}



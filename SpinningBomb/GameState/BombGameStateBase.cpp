// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BombGameStateBase.h"
#include "GameMode/BombGameModeBase.h"
#include "BombPlayerState.h"


#include "Net/UnrealNetwork.h"	//DOREPLIFETIME



ABombGameStateBase::ABombGameStateBase()
{
	GameModeClass = ABombGameModeBase::StaticClass();
	AuthorityGameMode = GameModeClass.GetDefaultObject();
	PlayerArray;
	NumOfPlayerInGame = 0;
	CurrentTurnOfPlayer = 0;
}



void ABombGameStateBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//HasBegunPlay();
}


void ABombGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABombGameStateBase, NumOfPlayerInGame);
	DOREPLIFETIME(ABombGameStateBase, CurrentTurnOfPlayer);
}




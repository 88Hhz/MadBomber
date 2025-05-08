// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "BombGameStateBase.generated.h"



/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ABombGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABombGameStateBase();
	
protected:

	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//virtual void OnRep_ReplicatedHasBegunPlay() override;

public:

	void SetNumPlayersInGame(const int32 NumOfPlayer) { NumOfPlayerInGame = NumOfPlayer; }
	const int32 GetNumPlayersInGame() { return NumOfPlayerInGame; }

	//Save Current turn of Player's Index, Called by GM
	void SetCurrentTurnOfPlayer(const int32 Index) { CurrentTurnOfPlayer = Index; }
	int32 GetCurrentTurnOfPlayer() { return CurrentTurnOfPlayer; }


	
protected:
	//Num of Participate in game
	UPROPERTY(Replicated)
	int32 NumOfPlayerInGame = 0;

	//Currnet player index of turn
	UPROPERTY(Replicated)
	uint8 CurrentTurnOfPlayer = 0;


	

private:

	

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BombGameModeBase.generated.h"

/**
 * 
 */
class ABombGameStateBase;

UCLASS()
class SPINNINGBOMB_API ABombGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABombGameModeBase();
	

protected:
	virtual void PostInitializeComponents() override;

	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	virtual void StartPlay() override;

public:
	void CheckPlayerToStart();

	//chat system
	void SendChatMessage(const FString& Message);

	//Call Spawned Bomb and Request CardEffect
	void ActivateCardEffect(const FName& CardKey);

	//Notify Requested Effect is finished, Called by SpawnedBomb
	void ActiveEffectsFinished(const int32 Damage, const int32 Count);

	// Logics Called by SpawnedBomb, Blue Cards
	void Logic_ReverseDirection();
	void Logic_JumpNextPlayer();
	void Logic_BlockCardEffect();
	void Logic_EliminateYellowCards();
	void Logic_ReduceNextPlayerCards();

	void DealCardsToPlayer();
	void ExplodeBomb();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "InGame")
	TArray<class APlayerStart*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "InGame")
	TSubclassOf<ACharacter> BombCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InGame")
	class UCardManager* CardManager;

	UPROPERTY(EditDefaultsOnly, category = "InGame")
	TSubclassOf<class ABomb> BombBP;

	UPROPERTY(BlueprintReadWrite, category = "InGame")
	ABomb* Bomb;
	void InitializeNewMatch();

	void NotifyUpdateWidget();

	void ChangeNextTurn();

private:
	class ABombPlayerController* GetPlayerControllerByPlayerId(int32 PlayerId);
	int32 NumOfPlayerInGame = 0;

	int32 SpawnIndex = 0;

	TArray<ABombPlayerController*> PCArray;

	bool bStartedMatch = false;

	int32 PreparedPlayer = 0;

	int32 NumInMatch = 0;

	int32 MatchRound = 0;

	//Properties for Turn
	bool bInverseTurn = false;
	int32 PlayerTurnIndex = 0;		//Current Player's Turn Index
	int32 TurnCalculator = 0;		//Sub Calculator for Current Player's Turn
	int32 TotalTurnCount = 0;		//Calculate Total Move Count in Match

	// Properties for Bomb
	int32 BombDamage = 0;
	int32 BombCount = 0;

	// Properties for CardEffects
	TArray<bool> YellowCard;
	int32 NumOfCards = 3;

	FTimerHandle EffectHandle;
};

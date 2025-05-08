// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "BombPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ABombPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABombPlayerState();
	

protected:
	virtual void PostInitializeComponents() override;

	virtual void CopyProperties(class APlayerState* PlayerState) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	// Functions used in LobbyGameMode
	void SetReady(bool NewReady);
	bool GetIsReady() { return bIsReady; }


	// Functions used in BombGameMode
	void SetMatchIndex(int32 NewIndex);
	int32 GetMatchIndex() { return MatchIndex; }

	//UTexture2D* GetPlayerAvatarTexture() { return SteamAvatarTexture; }


protected:
	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bIsReady = false;

	UFUNCTION()
	void OnRep_Ready();

	UPROPERTY(Replicated)
	int32 MatchIndex = 0;

private:

	/*UPROPERTY()
	UTexture2D* SteamAvatarTexture = nullptr;*/
};

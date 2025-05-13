// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerListRow.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ULobbyPlayerListRow : public UUserWidget
{
	GENERATED_BODY()
	

public:
	//void SetLobbyPlayerList(APlayerState* PlayerState);

	void SetPlayerName(const FString& Name);

	void ReadyForGame();

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ReadyState;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY()
	bool bIsReady = false;

private:


};

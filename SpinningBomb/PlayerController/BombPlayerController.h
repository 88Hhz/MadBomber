// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BombPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ABombPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABombPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* aPawn) override;

	virtual void BeginPlay() override;
	

public:
	FString GetUserName();
	UTexture2D* GetSteamAvatarTexture();

	UFUNCTION(BlueprintCallable)
	void ActivateChatUI();

	//Start Match System
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReadyforMatch();
	void Server_ReadyforMatch_Implementation();
	bool Server_ReadyforMatch_Validate();

	UFUNCTION(Client, Reliable)
	void Client_UpdateMainWidget();
	void Client_UpdateMainWidget_Implementation();

	//Chat Service
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendChatMessage(const FString& Message);
	bool Server_SendChatMessage_Validate(const FString& Message);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveChatMessage(const FString& Message);


	//InGame Card Deal System/ GM -> PC -> HUD -> CardWidget
	UFUNCTION(Client, Reliable, WithValidation)
	void Client_ReceiveDealtCards(const int32 Index, const TArray<FName>& DealtCards);
	void Client_ReceiveDealtCards_Implementation(const int32 Index, const TArray<FName>& DealtCards);
	bool Client_ReceiveDealtCards_Validate(const int32 Index, const TArray<FName>& DealtCards);

	//Player selected Card Info / CardWidget -> PC -> GM
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendSelectedCard(const FName& CardKey);
	void Server_SendSelectedCard_Implementation(const FName& CardKey);
	bool Server_SendSelectedCard_Validate(const FName& CardKey);



protected:



private:
	class UInputMappingContext* MappingContext;

};

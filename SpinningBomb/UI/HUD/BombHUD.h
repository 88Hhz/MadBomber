// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BombHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ABombHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABombHUD(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void ActivateChatUI();
	void AddChatMessage(const FString& Message);

	void UpdateMainWidget();

	void SetCardWidgetWithDealtCard(const TArray<FName>& DealtCards);
	void ClearCardWidget();

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> WBP_MainWidget;

	UPROPERTY()
	TSubclassOf<UUserWidget> WBP_ChatWidget;

	UPROPERTY()
	TSubclassOf<UUserWidget> WBP_CardWidget;

	//UPROPERTY()
	//TSubclassOf<UUserWidget> WBP_StateWidget;

private:
	UPROPERTY()
	class UBombMainWidget* MainWidget;

	UPROPERTY()
	class UChatWidget* ChatWidget;

	UPROPERTY()
	class UCardWidget* CardWidget;

	/*UPROPERTY()
	class UGameStateWidget* StateWidget;*/
};

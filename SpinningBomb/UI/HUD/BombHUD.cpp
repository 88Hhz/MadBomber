// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BombHUD.h"
#include "UI/ChatSystem/ChatWidget.h"

#include "UI/InGame/BombMainWidget.h"
#include "UI/InGame/GameStateWidget.h"
#include "UI/InGame/CardWidget.h"

#include "BombPlayerState.h"



ABombHUD::ABombHUD(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetBP(TEXT("WidgetBlueprint'/Game/InGame/UI/WBP_BombMainWidget.WBP_BombMainWidget_C'"));
	if (!ensure(MainWidgetBP.Class != nullptr)) return;
	WBP_MainWidget = MainWidgetBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> ChatWidgetBP(TEXT("'/Game/UI/ChatSystem/WBP_ChatWidget.WBP_ChatWidget_C'"));
	if (!ensure(ChatWidgetBP.Class != nullptr)) return;
	WBP_ChatWidget = ChatWidgetBP.Class;

	//ConstructorHelpers::FClassFinder<UUserWidget> StateWidgetBP(TEXT("'/Game/InGame/UI/WBP_GameStateWidget.WBP_GameStateWidget_C'"));
	//if (!ensure(StateWidgetBP.Class != nullptr)) return;
	//WBP_StateWidget = StateWidgetBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> CardWidgetBP(TEXT("WidgetBlueprint'/Game/InGame/UI/WBP_CardWidget.WBP_CardWidget_C'"));
	if (!ensure(CardWidgetBP.Class != nullptr)) return;
	WBP_CardWidget = CardWidgetBP.Class;
}


void ABombHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = GetOwningPlayerController();
	if (Controller)
	{
		MainWidget = CreateWidget<UBombMainWidget>(Controller, WBP_MainWidget);
		MainWidget->AddToViewport(1);

		ChatWidget = CreateWidget<UChatWidget>(Controller, WBP_ChatWidget);
		ChatWidget->AddToViewport(2);

		/*StateWidget = CreateWidget<UGameStateWidget>(Controller, WBP_StateWidget);
		StateWidget->AddToViewport();*/
		//StateWidget->WaitforGame();

		/*ABombPlayerState* PlayerState = Cast<ABombPlayerState>(Controller->PlayerState);
		if (PlayerState)
		{
			PlayerState->SetPlayerWidgetInterface(MainWidget);
		}*/
	}
}


void ABombHUD::ActivateChatUI()
{
	if (ChatWidget != nullptr)
	{
		ChatWidget->ActivateChatUI();
		UE_LOG(LogTemp, Warning, TEXT("BombHUD: ActivateChatUI()"));
	}
}

void ABombHUD::AddChatMessage(const FString& Message)
{
	if (ChatWidget != nullptr)
	{
		ChatWidget->AddChatMessage(Message);
	}
}

void ABombHUD::UpdateMainWidget()
{
	UE_LOG(LogTemp, Error, TEXT("BombHUD::UpdateMainWidget() Called."));

	if (MainWidget != nullptr)
	{
		MainWidget->SetPlayerStateList();
	}
}


void ABombHUD::SetCardWidgetWithDealtCard(const TArray<FName>& DealtCards)
{
	if (APlayerController* PC = GetOwningPlayerController())
	{
		if (CardWidget == nullptr)
		{
			CardWidget = CreateWidget<UCardWidget>(PC, WBP_CardWidget);
			CardWidget->AddToViewport(3);
		}
		CardWidget->SetWithDealtCards(DealtCards);
	}

}

void ABombHUD::ClearCardWidget()
{
	if (CardWidget != nullptr)
	{
		CardWidget->RemoveFromParent();
		CardWidget = nullptr;
	}
}

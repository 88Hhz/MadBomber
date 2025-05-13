﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyWidget.h"
#include "UI/Lobby/LobbyPlayerListRow.h"

#include "BombGameInstance.h"
#include "PlayerController/LobbyPlayerController.h"
#include "GameState/LobbyGameStateBase.h"

#include "BombPlayerState.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

#include "TimerManager.h"

#include "Kismet/GamePlayStatics.h"


ULobbyWidget::ULobbyWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> LobbyPlayerListRowBP(TEXT("'/Game/Lobby/UI/WBP_LobbyPlayerListRow.WBP_LobbyPlayerListRow_C'"));
	if (!ensure(LobbyPlayerListRowBP.Class != nullptr)) return;
	PlayerListRow = LobbyPlayerListRowBP.Class;
}

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ensure(ExitLobbyBtt != nullptr)) return;
	ExitLobbyBtt->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickedExitLobby);
	
	if (!ensure(GameStartBtt != nullptr)) return;
	GameStartBtt->OnClicked.AddDynamic(this, &ULobbyWidget::OnClickedGameStart);
}

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		LobbyName->SetText(FText::FromString(GI->GetCurrentLobbyName()));
	}


	if(APlayerController* PC = GetOwningPlayer())
	{
		//Game Start Buttom
		GameStartBtt->SetIsEnabled(!PC->HasAuthority());
		if (PC->HasAuthority())
		{
			GameStartBtt->SetVisibility(ESlateVisibility::Hidden);
			//CheckPlayerList();
		}
		else
		{
			GameStartBtt->SetVisibility(ESlateVisibility::Visible);
		}


		/*if (ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PC))
		{
			LPC->Server_GetPlayerList();
		}*/
	}
}

void ULobbyWidget::NativeDestruct()
{

}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

//void ULobbyWidget::CheckPlayerList()
//{
//	if (!bHost)
//	{
//		if (ALobbyGameStateBase* GS = GetWorld()->GetGameState<ALobbyGameStateBase>())
//		{
//			if (!bBind)
//			{
//				GS->OnPlayerArrayUpdated.AddDynamic(this, &ULobbyWidget::CheckPlayerList);
//				UE_LOG(LogTemp, Error, TEXT("LobbyWidget:: Bind Delegate"));
//				bBind = true;
//			}
//			else
//			{
//				GS->OnPlayerArrayUpdated.RemoveDynamic(this, &ULobbyWidget::CheckPlayerList);
//				bBind = false;
//
//				UpdatePlayerListWidget();
//				UE_LOG(LogTemp, Error, TEXT("LobbyWidget:: UnBind Delegate"));
//				
//			}
//		}
//	}
//	else
//	{
//		UpdatePlayerListWidget();
//	}
//}
//
//
//
//void ULobbyWidget::UpdatePlayerListWidget()
//{
//	if (!ensure(PlayerList != nullptr)) return;
//	PlayerList->ClearChildren();
//	this->PlayerArray.Empty();
//
//	ALobbyGameStateBase* GameState = GetWorld()->GetGameState<ALobbyGameStateBase>();
//	if (GameState)
//	{
//		for (APlayerState* State : GameState->PlayerArray)
//		{
//			ULobbyPlayerListRow* StateRow = CreateWidget<ULobbyPlayerListRow>(this, PlayerListRow);
//			if (!ensure(StateRow != nullptr))return;
//			StateRow->SetLobbyPlayerList(State);
//
//			this->PlayerArray.Add(StateRow);
//			PlayerList->AddChild(StateRow);
//		}
//	}
//}

void ULobbyWidget::UpdatePlayerList(const TArray<FString>& PlayerNameList)
{
	if (!ensure(PlayerList != nullptr)) return;
	PlayerList->ClearChildren();

	for (FString Name : PlayerNameList)
	{
		ULobbyPlayerListRow* ListRow = CreateWidget<ULobbyPlayerListRow>(this, PlayerListRow);
		//ListRow->SetPlayerList(Name);

		PlayerList->AddChild(ListRow);
	}
}

void ULobbyWidget::AddPlayerList(const FString& NewName)
{
	if (!ensure(PlayerList != nullptr)) return;

	ULobbyPlayerListRow* ListRow = CreateWidget<ULobbyPlayerListRow>(this, PlayerListRow);
	//ListRow->SetPlayerList(NewName);

	PlayerList->AddChild(ListRow);
}

void ULobbyWidget::EnableStartButton(bool CanStart)
{
	if (!CanStart)
	{
		GameStartBtt->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		GameStartBtt->SetVisibility(ESlateVisibility::Visible);
	}
	GameStartBtt->SetIsEnabled(CanStart);
}



void ULobbyWidget::OnClickedExitLobby()
{
	if(ALobbyPlayerController* Controller = GetOwningPlayer<ALobbyPlayerController>())
	{
		Controller->ExitCurrentLobby();
	}
}

void ULobbyWidget::OnClickedGameStart()
{
	if (ALobbyPlayerController* PC = GetOwningPlayer<ALobbyPlayerController>())
	{
		//같은 버튼이 클라이언트는 Ready
		if (!PC->HasAuthority())
		{
			bIsReady = !bIsReady;
			PC->Server_UpdateReadyState(bIsReady);
		}
		//서버는 GameStart 버튼
		else
		{
			PC->StartGame();
		}
	}
}

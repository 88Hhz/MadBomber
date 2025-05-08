// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/BombMainWidget.h"

#include "PlayerController/BombPlayerController.h"
//#include "BombGameInstance.h"
#include "GameState/BombGameStateBase.h"
#include "BombPlayerState.h"

#include "UI/InGame/PlayerStateRow.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

#include "TimerManager.h"

#include "Kismet/GameplayStatics.h"




UBombMainWidget::UBombMainWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerStateRowBP(TEXT("'/Game/InGame/UI/WBP_PlayerStateRow.WBP_PlayerStateRow_C'"));
	if (!ensure(PlayerStateRowBP.Class != nullptr)) return;
	
	PlayerStateRow = PlayerStateRowBP.Class;

}

void UBombMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}


void UBombMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>();
	if (PC)
	{
		PlayerAvatar->SetBrushFromTexture(PC->GetSteamAvatarTexture());
		PlayerName->SetText(FText::FromString(PC->GetUserName()));

		UE_LOG(LogTemp, Warning, TEXT("BombMainWidget:: NAVTIVE CONSTRUCT()"));
	}
	//CheckPlayerStateList();
}

void UBombMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UBombMainWidget::SetPlayerStateList()
{
	if (!ensure(PlayerList != nullptr)) return;
	PlayerList->ClearChildren();
	this->PlayerArray.Empty();

	UWorld* World = GetWorld();
	if(ABombGameStateBase* GS = World->GetGameState<ABombGameStateBase>())
	{
		for (APlayerState* State : GS->PlayerArray)
		{
			UPlayerStateRow* StateRow = CreateWidget<UPlayerStateRow>(this, PlayerStateRow);
			if (!ensure(StateRow != nullptr)) return;

			StateRow->SetPlayerStateBlock(State);

			UE_LOG(LogTemp, Error, TEXT("MainWIdget -> UpdatePlayerStateRow!!"));

			this->PlayerArray.Add(StateRow);

			PlayerList->AddChild(StateRow);
		}
	}
}

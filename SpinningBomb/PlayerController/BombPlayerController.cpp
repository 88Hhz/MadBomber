// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/BombPlayerController.h"
#include "BombPlayerState.h"

#include "GameMode/BombGameModeBase.h"
#include "GameState/BombGameStateBase.h"
#include "UI/HUD/BombHUD.h"

#include "BombGameInstance.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Kismet/GameplayStatics.h"



ABombPlayerController::ABombPlayerController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_DEFAULT(TEXT("'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
	if (!ensure(IMC_DEFAULT.Object != nullptr)) return;

	MappingContext = IMC_DEFAULT.Object;
}

void ABombPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


}


void ABombPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);


	UE_LOG(LogTemp, Error, TEXT("ABombPlayerController: OnPossess Fin. Ready for Match"));

	Server_ReadyforMatch();
}

void ABombPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (MappingContext)
		{
			SubSystem->AddMappingContext(MappingContext, 0);
		}
	}

	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;

}




FString ABombPlayerController::GetUserName()
{
	UBombGameInstance* GI = GetGameInstance<UBombGameInstance>();
	if (!ensure(GI != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("BombPC: Casting GameInstance is Failed."));
	}
	return GI->GetUserName();
}

UTexture2D* ABombPlayerController::GetSteamAvatarTexture()
{
	UBombGameInstance* GI = GetGameInstance<UBombGameInstance>();
	if (!ensure(GI != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("BombPC: Casting GameInstance is Failed."));
	}
	return GI->GetSteamAvatarTexture();
}


void ABombPlayerController::ActivateChatUI()
{
	ABombHUD* BombHUD = GetHUD<ABombHUD>();
	if (BombHUD)
	{
		BombHUD->ActivateChatUI();
		UE_LOG(LogTemp, Warning, TEXT("PlayerController: ActivateChatUI()"));
	}
}

//Start Match : Widget-> PC -> GM
void ABombPlayerController::Server_ReadyforMatch_Implementation()
{
	ABombGameModeBase* GM = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
	if (GM)
	{
		GM->CheckPlayerToStart();
		UE_LOG(LogTemp, Error, TEXT("BombGameController::ReadyforStart() Called."));
		//Error Point
	}
}

bool ABombPlayerController::Server_ReadyforMatch_Validate()
{
	return this->GetPlayerState<APlayerState>() != nullptr;
}



// Called by GameMode for Update Every Player's Match Information List
void ABombPlayerController::Client_UpdateMainWidget_Implementation()
{
	ABombHUD* BombHUD = GetHUD<ABombHUD>();
	if (BombHUD)
	{
		BombHUD->UpdateMainWidget();
		UE_LOG(LogTemp, Error, TEXT("BombPlayerController:: NotifyStartGame() Called."));
	}
}




//Chat System: Widget->PC->GM
void ABombPlayerController::Server_SendChatMessage_Implementation(const FString& Message)
{
	ABombPlayerState* BombPlayerState = this->GetPlayerState<ABombPlayerState>();
	if (BombPlayerState)
	{
		FString NewMessage = FString::Printf(TEXT("%s: %s"), *BombPlayerState->GetPlayerName(), *Message);
		//Add Owning Player's Name

		ABombGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
		if (GameMode)
		{
			GameMode->SendChatMessage(NewMessage);
		}
	}
}

bool ABombPlayerController::Server_SendChatMessage_Validate(const FString& Message)
{
	return !Message.IsEmpty();
}

//Chat System:GM -> PC -> HUD -> Widget
void ABombPlayerController::Client_ReceiveChatMessage_Implementation(const FString& Message)
{
	ABombHUD* BombHUD = GetHUD<ABombHUD>();
	if (BombHUD)
	{
		BombHUD->AddChatMessage(Message);
	}
}


void ABombPlayerController::Client_ReceiveDealtCards_Implementation(const int32 Index, const TArray<FName>& DealtCards)
{
	ABombHUD* BombHUD = GetHUD<ABombHUD>();
	if (BombHUD)
	{
		BombHUD->SetCardWidgetWithDealtCard(DealtCards);
	}
	UE_LOG(LogTemp, Warning, TEXT("BombPlayerController : ReceiveCards : DealtCards : %d"), DealtCards.Num());
}

bool ABombPlayerController::Client_ReceiveDealtCards_Validate(const int32 Index, const TArray<FName>& DealtCards)
{
	UE_LOG(LogTemp, Warning, TEXT("Client_ReceiveDealtCards Playerid : %d "), this->GetPlayerState<APlayerState>()->GetPlayerId());
	if (this->GetPlayerState<APlayerState>()->GetPlayerId() == Index)
	{
		return true;
	}
	return false;
}

void ABombPlayerController::Server_SendSelectedCard_Implementation(const FName& CardKey)
{
	ABombGameModeBase* GM = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
	if (GM)
	{
		GM->ActivateCardEffect(CardKey);
		UE_LOG(LogTemp, Error, TEXT("BombGameController::SendSelectedCard() Called."));
	}
}

bool ABombPlayerController::Server_SendSelectedCard_Validate(const FName& CardKey)
{

	UE_LOG(LogTemp, Warning, TEXT("BombGameController:Server_SendSelectedCard' CardKey : %s "), *CardKey.ToString());
	return CardKey.IsValid();
}




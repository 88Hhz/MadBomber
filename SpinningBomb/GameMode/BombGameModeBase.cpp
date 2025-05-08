// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BombGameModeBase.h"
#include "BombGameInstance.h"

#include "GameState/BombGameStateBase.h"

#include "UI/HUD/BombHUD.h"
#include "Actor/Bomb/Bomb.h"

#include "Object/Card/CardManager.h"

#include "PlayerController/BombPlayerController.h"
#include "BombPlayerState.h"

#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


ABombGameModeBase::ABombGameModeBase()
{
	DefaultPawnClass = nullptr;

	PlayerControllerClass = ABombPlayerController::StaticClass();

	//static ConstructorHelpers::FClassFinder<APawn> BombPlayerPawnBP(TEXT("'/Game/BombMain/BP_BombCharacter.BP_BombCharacter_C'"));
	//if (BombPlayerPawnBP.Class != NULL)
	//{
	//	DefaultPawnClass = BombPlayerPawnBP.Class;
	//}

	HUDClass = ABombHUD::StaticClass();
	GameStateClass = ABombGameStateBase::StaticClass();
	PlayerStateClass = ABombPlayerState::StaticClass();

	bStartedMatch = false;
	PreparedPlayer = 0;
	NumInMatch = 0;
	PlayerTurnIndex = 0;
	TurnCalculator = 0;
}

void ABombGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);

	SpawnPoints.Empty();
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		SpawnPoints.Add(*It);
	}
}

void ABombGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

}

void ABombGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("HandleStartingNewPlayer_Implementation()"));

	if (ABombPlayerController* PC = Cast<ABombPlayerController>(NewPlayer))
	{
		FTransform SpawnTr = SpawnPoints[SpawnIndex++]->GetActorTransform();
		ACharacter* Character = GetWorld()->SpawnActor<ACharacter>(BombCharacter, SpawnTr.GetLocation(), SpawnTr.GetRotation().Rotator());

		PCArray.Add(PC);

		PC->Possess(Character);

		if (ABombPlayerState* PS = Cast<ABombPlayerState>(PC->PlayerState))
		{
			PS->SetMatchIndex(SpawnIndex);
			PS->SetPlayerId(SpawnIndex);
		}
	}
}


void ABombGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	/*if (Exiting->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("BombGameMode: Host is Logout"));
	}
	else
	{
		if (ABombGameStateBase* BombGameState = GetGameState<ABombGameStateBase>())
		{
			BombGameState->RemovePlayerState(Exiting->PlayerState);
			BombGameState->SetNumPlayersInGame(GetNumPlayers());
			UE_LOG(LogTemp, Warning, TEXT("BombGameMode: Client is Logout, NumOfPlayer Diminished"));
		}
	}
	--NumPlayers;

	UBombGameInstance* GameInstance = GetGameInstance<UBombGameInstance>();
	if (GameInstance)
	{
		GameInstance->SetNumPlayersInGame(GetNumPlayers());
	}*/

	NotifyUpdateWidget();
}

void ABombGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (UBombGameInstance* GI = GetGameInstance<UBombGameInstance>())
	{
		NumOfPlayerInGame = GI->GetNumOfPlayerInGame();
	}
}

void ABombGameModeBase::StartPlay()
{
	

	/*if (ABombGameStateBase* GS = GetGameState<ABombGameStateBase>())
	{
		GS->SetNumPlayersInGame(GetNumPlayers());
	}*/
}



void ABombGameModeBase::CheckPlayerToStart()
{
	if (!bStartedMatch)
	{
		++PreparedPlayer;
		UE_LOG(LogTemp, Error, TEXT("BombGameMode::ReadyforStart() Called. : %d "), PreparedPlayer);

		if (PreparedPlayer >= NumOfPlayerInGame)
		{
			bStartedMatch = true;

			if (ABombGameStateBase* GS = GetGameState<ABombGameStateBase>())
			{
				GS->SetNumPlayersInGame(NumOfPlayerInGame);
			}
			Super::StartPlay();
			NotifyUpdateWidget();
			InitializeNewMatch();
		}
	}
}





//Chat System
void ABombGameModeBase::SendChatMessage(const FString& Message)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABombPlayerController* Controller = Cast<ABombPlayerController>(*It);
		if (Controller)
		{
			Controller->Client_ReceiveChatMessage(Message);
		}
	}
}


//Send CardEffect to Bomb And change turn
void ABombGameModeBase::ActivateCardEffect(const FName& CardKey)
{
	if (Bomb)
	{
		Bomb->ActivateCardEffect(CardKey);
	}
}

//CallBack From SpawnedBomb After Effect
void ABombGameModeBase::ActiveEffectsFinished(const int32 Damage, const int32 Count)
{
	UE_LOG(LogTemp, Warning, TEXT("BombGameMode:: Bomb Effect is Finished"));
	this->BombDamage = Damage;
	this->BombCount = Count;

	//추후 수정
	GetWorld()->GetTimerManager().SetTimer(EffectHandle, this, &ABombGameModeBase::ChangeNextTurn, 3.f, false);
	UE_LOG(LogTemp, Warning, TEXT("BombGameMode:: Bomb Effect is Finished And Make Delay"));
	//ChangeTurnAndDealCards();
}

//Make Turn Direction Inverse
void ABombGameModeBase::Logic_ReverseDirection()
{
	bInverseTurn = !bInverseTurn;
}

//Next Player's turn is Skipped
void ABombGameModeBase::Logic_JumpNextPlayer()
{
	++TurnCalculator;
}

//1~3 Players Can't Select Card
void ABombGameModeBase::Logic_BlockCardEffect()
{

}

//Eliminate All Yellow Card
void ABombGameModeBase::Logic_EliminateYellowCards()
{
	//추후 해당 애님추가
	YellowCard.Init(false, 4);
}

//Reduce
void ABombGameModeBase::Logic_ReduceNextPlayerCards()
{

}


// Called by Bomb : AnimNotify_MoveFin
void ABombGameModeBase::DealCardsToPlayer()
{
	ABombPlayerController* PC = GetPlayerControllerByPlayerId(PlayerTurnIndex);
	if (PC != nullptr)
	{
		PC->Client_ReceiveDealtCards(PlayerTurnIndex, CardManager->DealCards());
	}
}

void ABombGameModeBase::ExplodeBomb()
{
	if (Bomb)
	{
		Bomb->Destroy();
		//particle 추후 추가
	}

	//데미지 시스템
	MatchRound++;
}



void ABombGameModeBase::InitializeNewMatch()
{
	UE_LOG(LogTemp, Error, TEXT("ABombGameModeBase::GetNumPlayers() = %d"), GetNumPlayers());

	if (BombBP)
	{
		FVector SpawnLocation = FVector(1540.f, 1790.f, 100.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		Bomb = GetWorld()->SpawnActor<ABomb>(BombBP, SpawnLocation, SpawnRotation);
		UE_LOG(LogTemp, Log, TEXT("Bomb Spawned"));
	}

	CardManager = NewObject<UCardManager>(this, UCardManager::StaticClass());
	if (CardManager != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("CardManager Spawned"));
		CardManager->InitializeCardPool();
	}

	YellowCard.Init(false, 4);

	//초기 테스트
	NumInMatch = NumOfPlayerInGame;

	GetWorld()->GetTimerManager().SetTimer(EffectHandle, this, &ABombGameModeBase::ChangeNextTurn, 2.f, false);
}

void ABombGameModeBase::NotifyUpdateWidget()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ABombPlayerController* PC = Cast<ABombPlayerController>(*It))
		{
			PC->Client_UpdateMainWidget();
			UE_LOG(LogTemp, Error, TEXT("BombGameMode:: UpdatePlayerList() Called."));
		}
	}
}



void ABombGameModeBase::ChangeNextTurn()
{
	//Error Point
	if (ABombGameStateBase* BombGameState = GetGameState<ABombGameStateBase>())
	{
		//Prepare Negative : Modular
		PlayerTurnIndex = FMath::Clamp((TurnCalculator % NumInMatch + NumInMatch) % NumInMatch + 1, 1, NumInMatch);
		BombGameState->SetCurrentTurnOfPlayer(PlayerTurnIndex);
		UE_LOG(LogTemp, Warning, TEXT("ChangeCurrentTurn(): Next PlayerID: %d"), PlayerTurnIndex);
	}

	if (bInverseTurn)
	{
		++TurnCalculator;
	}
	else
	{
		--TurnCalculator;
	}
	++TotalTurnCount;

	//Bomb->StartMoveToNextPlayer(PlayerTurnIndex);

	//Bomb Location Test
	Bomb->StartMoveToNextPlayer(TotalTurnCount % 4 + 1);


	//Bomb Anim 이후, Called DealCardsToPlayer()
}



// Find BombPlayerController : Currnet Index From PlayerState
ABombPlayerController* ABombGameModeBase::GetPlayerControllerByPlayerId(int32 PlayerId)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;

		APlayerState* PS = PC->PlayerState;
		if (PS->GetPlayerId() == PlayerId)
		{
			ABombPlayerController* BombPC = Cast<ABombPlayerController>(PC);
			if (BombPC)
			{
				return BombPC;
			}
		}
	}
	return nullptr;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"

#include "BombGameInstance.h"

#include "GameState/LobbyGameStateBase.h"
#include "PlayerController/LobbyPlayerController.h"

#include "Actor/Character/LobbyCharacter.h"
#include "BombPlayerState.h"


#include "EngineUtils.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"




ALobbyGameMode::ALobbyGameMode()
{
	DefaultPawnClass = nullptr;
	bStartPlayersAsSpectators = true;

	LobbyCamera = nullptr;
	
}

void ALobbyGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	if (Found != nullptr)
	{
		LobbyCamera = Cast<ACameraActor>(Found);
	}
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);

	SpawnPoints.Empty();
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		SpawnPoints.Add(*It);
	}
	bSpawnSlot.Init(false, SpawnPoints.Num());
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Error, TEXT("Player InLobby: %d"), ++NumOfPlayers);

	if (!LobbyCamera) return;

	int32 SlotIndex = FindEmptySlot();
	if (SlotIndex < 0) return;

	bSpawnSlot[SlotIndex] = true;
	PCMap.Add(NewPlayer, SlotIndex);

	FTransform SpawnTr = SpawnPoints[SlotIndex]->GetActorTransform();
	ACharacter* Character = GetWorld()->SpawnActor<ACharacter>(LobbyCharacter, SpawnTr.GetLocation(), SpawnTr.GetRotation().Rotator());

	CharacterMap.Add(SlotIndex, Character);

	NewPlayer->Possess(Character);

	NewPlayer->bAutoManageActiveCameraTarget = false;
	NewPlayer->SetViewTarget(LobbyCamera);

	/*if (ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(NewPlayer))
	{
		const FString NewName = PC->PlayerState->GetPlayerName();

		for (ALobbyPlayerController* LobbyPC : LobbyPCs)
		{
			LobbyPC->Client_AddPlayerList(NewName);
		}

		LobbyPCs.AddUnique(PC);
		PlayerNameList.Add(NewName);
	}*/
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UE_LOG(LogTemp, Error, TEXT("Player LogOut: %d"), --NumOfPlayers);

	if (APlayerController* PC = Cast<APlayerController>(Exiting))
	{
		if (int32* Slot = PCMap.Find(PC))
		{
			int32 SlotIndex = *Slot;
			if (ACharacter** Character = CharacterMap.Find(SlotIndex))
			{
				(*Character)->Destroy();
				CharacterMap.Remove(SlotIndex);
			}
			bSpawnSlot[SlotIndex] = false;
			PCMap.Remove(PC);
		}
	}
}




void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

void ALobbyGameMode::CheckAllPlayerReadyState()
{
	ALobbyPlayerController* HostPC = nullptr;
	bool bCanStartMatch = true;

	for (auto& Map : PCMap)
	{
		//컨트롤러마다 부여된 Index로 확인
		APlayerController* PC = Map.Key;
		int32 SlotIndex = Map.Value;

		//서버(호스트)는 레디유무 체크 제외
		if (SlotIndex == 0)
		{
			HostPC = Cast<ALobbyPlayerController>(PC);
			continue;
		}

		if (ABombPlayerState* PS = Cast<ABombPlayerState>(PC->PlayerState))
		{
			//한명이라도 레디상태 아니면 버튼 비활성화
			if (!PS->GetIsReady())
			{
				bCanStartMatch = false;
				break;
			}
		}
	}

	if (HostPC != nullptr)
	{
		//PlayerController에서 Widget으로 전달
		HostPC->EnableStartButton(bCanStartMatch);
	}
}

void ALobbyGameMode::StartGame()
{
	UE_LOG(LogTemp, Log, TEXT("GameMode: GameStart!!"));

	UBombGameInstance* GameInstance = GetGameInstance<UBombGameInstance>();
	if (GameInstance)
	{
		GameInstance->SetNumOfPlayerInGame(NumOfPlayers);
	}

	bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
}


int32 ALobbyGameMode::FindEmptySlot() const
{
	for (int i = 0; i < bSpawnSlot.Num(); ++i)
	{
		if (!bSpawnSlot[i])
			return i;
	}
	return -1;
}

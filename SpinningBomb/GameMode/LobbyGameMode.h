// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "LobbyGameMode.generated.h"



/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();

protected:
	virtual void PostInitializeComponents() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	

	virtual void BeginPlay() override;
	
public:
	//void UpdatePlayerWidget();

	void CheckAllPlayerReadyState();

	void StartGame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Lobby")
	ACameraActor* LobbyCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Lobby")
	TSubclassOf<ACharacter> LobbyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Lobby")
	TArray<class APlayerStart*> SpawnPoints;

private:
	uint8 NumOfPlayers = 0;

	TArray<bool> bSpawnSlot;

	TMap<APlayerController*, int32> PCMap;

	TMap<int32, ACharacter*> CharacterMap;

	int32 FindEmptySlot() const;

	UPROPERTY()
	TArray<FString> PlayerNameList;

};

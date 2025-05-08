// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#pragma warning(push)
#pragma warning(disable:4996)
#include "ThirdParty/Steamworks/Steamv157/sdk/public/steam/steam_api.h"
#pragma warning(pop)
//#pragma comment(lib, "ThirdParty/Steamworks/Steamv161/sdk/redistributable_bin/win64/steam_api64.lib")

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Session/SessionDataStruct.h"

#include "BombGameInstance.generated.h"



DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFindSessionCompleted, const TArray<FSessionDataStruct>, SessionDatas);

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UBombGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBombGameInstance();

protected:
	virtual void Init() override;


public:
	UFUNCTION(BlueprintCallable, category = "Session")
	virtual void CreateSession(int32 NumPublicConnections, FString LobbyName, bool bFriendOnly);

	UFUNCTION(BlueprintCallable, category = "Session")
	virtual void DestroySession();

	UFUNCTION(BlueprintCallable, category = "Session")
	virtual void FindCreatedSession();

	UFUNCTION(BlueprintCallable, category = "Session")
	virtual void JoinCreatedSession();

	UFUNCTION(BlueprintCallable, category = "Session")
	virtual void JoinSelectedSession(const int32 Index);

	bool IsPlayerInSession();

	FString GetCurrentLobbyName() { return this->CurrentLobbyName; }

	UFUNCTION(BlueprintCallable, category = "UserData")
	FString GetUserName() { return UserName.ToString(); }

	UFUNCTION(BlueprintCallable, category = "UserData")
	void SetUniqueNetId(const FUniqueNetIdRepl& NewUniqueNetId) { this->UniqueNetId = NewUniqueNetId; }

	const FUniqueNetIdRepl& GetUniqueNetId() { return UniqueNetId; }

	UFUNCTION(BlueprintCallable, category = "UserData")
	UTexture2D* GetSteamAvatarTexture() { return SteamAvatarTexture; }

	void SetNumOfPlayerInGame(int32 NewNum) { this->NumOfPlayerInGame = NewNum; }
	int32 GetNumOfPlayerInGame() { return this->NumOfPlayerInGame; }

	bool IsInLobby() { return bIsInLobby; }

	FOnFindSessionCompleted OnFindSessionCompleted;
protected:
	

private:
	void InitializeSteam();

	UFUNCTION()
	UTexture2D* SetSteamAvatarTexture();

	//static constexpr char* APP_ID = STEAM_APP_ID;
	CSteamID SteamID;

	UPROPERTY()
	FText UserName;

	FUniqueNetIdRepl UniqueNetId;

	bool bIsInLobby = false;

	UPROPERTY()
	UTexture2D* SteamAvatarTexture;

	UPROPERTY()
	int32 NumOfPlayerInGame = 0;


	FString CurrentLobbyName;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnFindSessionComplete(bool bWasSuccessful);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};

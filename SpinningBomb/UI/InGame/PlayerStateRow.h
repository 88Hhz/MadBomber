// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateRow.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UPlayerStateRow : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetPlayerStateBlock(APlayerState* PlayerState);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerIndex;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PlayerHpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerCoin;

private:


};

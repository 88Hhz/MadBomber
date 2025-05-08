// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BombMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UBombMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBombMainWidget(const FObjectInitializer& ObjectInitializer);

	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, category = "PlayerList")
	void SetPlayerStateList();


protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerAvatar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerHp;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* PlayerList;

	

private:
	UPROPERTY()
	TArray<class UPlayerStateRow*> PlayerArray;

	UPROPERTY()
	TSubclassOf<UUserWidget> PlayerStateRow;


	uint8 CurrentHp = 20;
	uint8 CurrentCoin = 5;

	FTimerHandle UpdateTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UCardWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UCardWidget(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:
	UFUNCTION(BlueprintCallable, category = "Card")
	void SetWithDealtCards(const TArray<FName>& DealtCards);

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* Card1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Card2;

	UPROPERTY(meta = (BindWidget))
	class UButton* Card3;


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name3;


private:
	TArray<struct FCardData> HandedCards;
	TArray<FName> CardKeys;

	UFUNCTION()
	void SetCardButtons();

	UFUNCTION()
	void OnClickedSelectedCard1();

	UFUNCTION()
	void OnClickedSelectedCard2();

	UFUNCTION()
	void OnClickedSelectedCard3();

	UFUNCTION()
	void RemoveCardButtons();
};

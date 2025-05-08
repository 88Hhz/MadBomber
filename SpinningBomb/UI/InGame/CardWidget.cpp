// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/CardWidget.h"
#include "Object/Card/CardDataTable.h"
#include "UI/HUD/BombHUD.h"
#include "PlayerController/BombPlayerController.h"


#include "Components/Button.h"
#include "Components/TextBlock.h"


UCardWidget::UCardWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{


}

void UCardWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}

void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCardWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

void UCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCardWidget::SetWithDealtCards(const TArray<FName>& DealtCards)
{
	//추후 수정
	//DealtCards.Num()확인해서 2~3장 확인하기.

	
	UE_LOG(LogTemp, Warning, TEXT("CardWidget::SetWithDealtCards : DealCards : %d "), DealtCards.Num());
	if (DealtCards.Num() > 0)
	{
		CardKeys = DealtCards;
		HandedCards.Empty();


		UDataTable* CardDataTable = LoadObject<UDataTable>(this, TEXT("DataTable'/Game/InGame/Card/DT_CardDataTable.DT_CardDataTable'"));
		if (CardDataTable)
		{
			TArray<FName> RowNames = CardDataTable->GetRowNames();
			static const FString ContextString(TEXT("DataContext"));

			for (const FName& Card : DealtCards)
			{
				FCardData* CardData = CardDataTable->FindRow<FCardData>(Card, ContextString, true);
				if (CardData)
				{
					HandedCards.Add(*CardData);
				}
			}
		}
		SetCardButtons();
	}
	
}

void UCardWidget::SetCardButtons()
{
	//추후 애니메이션 추가
	Name1->SetText(FText::FromString(HandedCards[0].CardName));
	Name2->SetText(FText::FromString(HandedCards[1].CardName));
	Name3->SetText(FText::FromString(HandedCards[2].CardName));

	TMap<ECardType, FLinearColor> CardColorMap;
	CardColorMap.Add(ECardType::Red, FLinearColor::Red);
	CardColorMap.Add(ECardType::Blue, FLinearColor::Blue);
	CardColorMap.Add(ECardType::Yellow, FLinearColor::Yellow);
	CardColorMap.Add(ECardType::Purple, FLinearColor::Green);


	//SetColor From CardDataType
	Card1->SetBackgroundColor(*CardColorMap.Find(HandedCards[0].CardType));
	Card2->SetBackgroundColor(*CardColorMap.Find(HandedCards[1].CardType));
	Card3->SetBackgroundColor(*CardColorMap.Find(HandedCards[2].CardType));

	Card1->OnClicked.AddDynamic(this, &UCardWidget::OnClickedSelectedCard1);
	Card2->OnClicked.AddDynamic(this, &UCardWidget::OnClickedSelectedCard2);
	Card3->OnClicked.AddDynamic(this, &UCardWidget::OnClickedSelectedCard3);
}

void UCardWidget::OnClickedSelectedCard1()
{
	ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>();
	if (PC)
	{
		PC->Server_SendSelectedCard(CardKeys[0]);
		
	}
	RemoveCardButtons();
}

void UCardWidget::OnClickedSelectedCard2()
{
	ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>();
	if (PC)
	{
		PC->Server_SendSelectedCard(CardKeys[1]);
	}
	RemoveCardButtons();
}

void UCardWidget::OnClickedSelectedCard3()
{
	ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>();
	if (PC)
	{
		PC->Server_SendSelectedCard(CardKeys[2]);
	}
	RemoveCardButtons();
}

void UCardWidget::RemoveCardButtons()
{
	Card1->OnClicked.RemoveDynamic(this, &UCardWidget::OnClickedSelectedCard1);
	Card2->OnClicked.RemoveDynamic(this, &UCardWidget::OnClickedSelectedCard2);
	Card3->OnClicked.RemoveDynamic(this, &UCardWidget::OnClickedSelectedCard3);

	ABombPlayerController* PC = GetOwningPlayer<ABombPlayerController>();
	if (ABombHUD* HUD = PC->GetHUD<ABombHUD>())
	{
		HUD->ClearCardWidget();
	}
}

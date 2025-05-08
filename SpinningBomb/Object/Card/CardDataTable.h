// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CardDataTable.generated.h"


UENUM(BlueprintType)
enum class ECardType : uint8 {
    Red,
    Blue,
    Yellow,
    Purple
};

USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    int32 CardID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    ECardType CardType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString CardName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FString CardDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    UTexture2D* CardTexture;
};



/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UCardDataTable : public UDataTable
{
	GENERATED_BODY()
	
};

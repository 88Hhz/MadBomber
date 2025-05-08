// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Actor/Bomb/Bomb.h"

#include "BombAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPINNINGBOMB_API UBombAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UBombAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:


public:
	UFUNCTION(BlueprintCallable, category = "Bomb")
	void SetBombState(EBombState NewState) { BombState = NewState; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Bomb")
	EBombState BombState;

private:



};

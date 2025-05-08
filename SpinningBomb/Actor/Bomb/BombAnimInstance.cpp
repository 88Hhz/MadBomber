// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Bomb/BombAnimInstance.h"

UBombAnimInstance::UBombAnimInstance()
{
	BombState = EBombState::Initial;

}

void UBombAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UBombAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

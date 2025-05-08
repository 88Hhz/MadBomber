// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"


UENUM(BlueprintType)
enum class EBombState : uint8 {
	Initial,
	Idle,
	Move,
	Explode
};




UCLASS()
class SPINNINGBOMB_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called by GM
	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void StartMoveToNextPlayer(int32 PlayerId);

	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void OnArrivedNextPlayer(float How);

	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void OnTriggeredExplode();

	// Called by GM
	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void ActivateCardEffect(const FName& CardKey);

protected:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComp;

	UMaterialInstanceDynamic* EmissiveMaterial;

	UPROPERTY(BlueprintReadWrite, category = "BombState")
	EBombState BombState;

	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void RedEffectLogic(int32 CardID);

	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void BlueEffectLogic(int32 CardID);

	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void YellowEffectLogic(int32 CardID);



	UFUNCTION(BlueprintCallable, category = "BombFunction")
	void EmissiveBombColor(FLinearColor Color);

	void StopEmissive();



private:
	UPROPERTY()
	int32 Count = 0;

	UPROPERTY()
	int32 Damage = 0;

	FVector MoveStartLocation;
	FVector MoveTargetLocation;

	float ElapsedTime = 0.f;
	float MoveDuration = 2.f;
	bool bIsMove = false;

	FTimerHandle EmissiveHandle;
};

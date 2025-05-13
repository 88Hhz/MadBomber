// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LobbyCharacter.generated.h"



UCLASS()
class SPINNINGBOMB_API ALobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALobbyCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	void SetPlayerName();

	UFUNCTION()
	void UpdateReadyState();

	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* PlayerWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerWidget;

};

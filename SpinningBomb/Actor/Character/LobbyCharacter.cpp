// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/LobbyCharacter.h"
#include "BombGameInstance.h"

#include "BombPlayerState.h"
#include "UI/Lobby/LobbyPlayerListRow.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALobbyCharacter::ALobbyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	GetMesh()->SetIsReplicated(true);

	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (PlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
	}

	PlayerWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerWidgetComp"));
	PlayerWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	PlayerWidgetComp->SetupAttachment(GetMesh());
	PlayerWidgetComp->SetRelativeLocation(FVector(0.f, 50.f, 10.f));
	PlayerWidgetComp->SetDrawSize(FVector2D(200, 50));
	PlayerWidgetComp->SetIsReplicated(true);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerWidget != nullptr)
	{
		PlayerWidgetComp->SetWidgetClass(PlayerWidget);
		SetPlayerName();
	}
}

// Called every frame
void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALobbyCharacter::SetPlayerName()
{
	if (PlayerWidgetComp)
	{
		if (UUserWidget* UW = PlayerWidgetComp->GetUserWidgetObject())
		{
			if (ULobbyPlayerListRow* ListRow = Cast<ULobbyPlayerListRow>(UW))
			{
				if (APlayerController* PC = GetController<APlayerController>())
				{
					UE_LOG(LogTemp, Warning, TEXT("UserNAme is::  %s"), *PC->PlayerState->GetPlayerName());
					ListRow->SetPlayerName(PC->PlayerState->GetPlayerName());
				}
			}
		}
	}
}

void ALobbyCharacter::UpdateReadyState()
{
	if (PlayerWidgetComp)
	{
		if (UUserWidget* UW = PlayerWidgetComp->GetUserWidgetObject())
		{
			if (ULobbyPlayerListRow* ListRow = Cast<ULobbyPlayerListRow>(UW))
			{
				ListRow->ReadyForGame();
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/BombCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/CharacterMovementComponent.h"


ABombCharacter::ABombCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	GetMesh()->SetIsReplicated(true);

	// ī�޶� ���� (CapsuleComponent�� ����)
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(17.f, 0.f, 78.f)); // �Ӹ� ��ġ
	CameraComponent->SetRelativeRotation(FRotator(0.f, 0.f, -20.f));
	CameraComponent->bUsePawnControlRotation = true;

	// ��ü ĳ���� �޽� (3��Ī��)
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

	// �޽� ���ҽ� �ε�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (PlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
	}

	// ��Ʈ�� ����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void ABombCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABombCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABombCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


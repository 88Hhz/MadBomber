// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Bomb/Bomb.h"
#include "GameMode/BombGameModeBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Actor/Bomb/BombAnimInstance.h"	//Bomb Anim

#include "Object/Card/CardDataTable.h"

#include "TimerManager.h"



ABomb::ABomb(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	//bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMeshComp;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BombSM(TEXT("'/Game/Asset/Bomb/ANM_Bomb_Idle.ANM_Bomb_Idle'"));
	if (BombSM.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(BombSM.Object);
	}
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
	BombState = EBombState::Initial;
	//초기 애니메이션 -> 노티파이 넣기

	if (SkeletalMeshComp)
	{
		EmissiveMaterial = SkeletalMeshComp->CreateAndSetMaterialInstanceDynamic(5);

		if (!EmissiveMaterial)
		{
			UE_LOG(LogTemp, Error, TEXT("DynamicMaterial Failed"));
		}
	}

	Damage = FMath::RandRange(3, 5);
	Count = FMath::RandRange(10, 20);

	UE_LOG(LogTemp, Warning, TEXT("Bomb: %d Damage is Set"), Damage);
	UE_LOG(LogTemp, Warning, TEXT("Bomb: %d Count is Set"), Count);
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMove)
	{
		ElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(ElapsedTime / MoveDuration, 0.f, 1.f);

		FVector NewLocation = FMath::Lerp(MoveStartLocation, MoveTargetLocation, Alpha);
		SetActorLocation(NewLocation);

		if (Alpha >= 1.f)
		{
			bIsMove = false;

			// → AnimNotify Will Trigger/ 임시 테스트용
			//OnArrivedNextPlayer();
		}
	}

}


void ABomb::StartMoveToNextPlayer(int32 PlayerId)
{
	BombState = EBombState::Move;

	UAnimInstance* AI = SkeletalMeshComp->GetAnimInstance();
	if (UBombAnimInstance* BAI = Cast<UBombAnimInstance>(AI))
	{
		BAI->SetBombState(this->BombState);
	}
	
	MoveStartLocation = GetActorLocation();

	switch (PlayerId)
	{
	case 1:
		MoveTargetLocation = FVector(1390.f, 1810.f, 100.f);
		break;
	case 2:
		MoveTargetLocation = FVector(1550.f, 1660.f, 100.f);
		break;
	case 3:
		MoveTargetLocation = FVector(1690.f, 1810.f, 100.f);
		break;
	case 4:
		MoveTargetLocation = FVector(1550.f, 1950.f, 100.f);
		break;
	default:
		MoveTargetLocation = FVector(1390.f, 1810.f, 100.f);
		break;
	}

	ElapsedTime = 0.f;
	bIsMove = true;
}

void ABomb::OnArrivedNextPlayer(float How)
{
	FRotator td = GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("Bomb: Previous Rotate: %f"), td.Yaw);


	td = td + FRotator(0.f, How, 0.f);
	SetActorRelativeRotation(td);
	UE_LOG(LogTemp, Warning, TEXT("Bomb: New Rotate: %f"), td.Yaw);



	BombState = EBombState::Idle;
	UAnimInstance* AI = SkeletalMeshComp->GetAnimInstance();
	if (UBombAnimInstance* BAI = Cast<UBombAnimInstance>(AI))
	{
		BAI->SetBombState(this->BombState);
	}

	if (--Count > 0)
	{
		ABombGameModeBase* GM = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
		if (GM)
		{
			GM->DealCardsToPlayer();
		}
	}
	else
	{
		BombState = EBombState::Explode;
		//UAnimInstance* AI = SkeletalMeshComp->GetAnimInstance();
		if (UBombAnimInstance* BAI = Cast<UBombAnimInstance>(AI))
		{
			BAI->SetBombState(this->BombState);
		}
	}

}

void ABomb::OnTriggeredExplode()
{
	ABombGameModeBase* GM = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
	if (GM)
	{
		GM->ExplodeBomb();
	}
}

//void ABomb::OnArrivedNextPlayer()
//{
//	
//	FRotator td = GetActorRotation();
//	UE_LOG(LogTemp, Warning, TEXT("Bomb: Previous Rotate: %d, %d, %d"), td.Pitch, td.Yaw, td.Roll);
//
//
//	td = td + FRotator(0.f, 90.f, 0.f);
//	SetActorRelativeRotation(td);
//	UE_LOG(LogTemp, Warning, TEXT("Bomb: New Rotate: %d, %d, %d"), td.Pitch, td.Yaw, td.Roll);
//
//	
//
//	BombState = EBombState::Idle;
//	UAnimInstance* AI = SkeletalMeshComp->GetAnimInstance();
//	if (UBombAnimInstance* BAI = Cast<UBombAnimInstance>(AI))
//	{
//		BAI->SetBombState(this->BombState);
//	}
//
//	ABombGameModeBase* GM = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
//	if (GM)
//	{
//		GM->DealCardsToPlayer();
//	}
//}

void ABomb::ActivateCardEffect(const FName& CardKey)
{
	//매번 로드할 것인지 체크해보기
	//UDataTable* CardDataTable = LoadObject<UDataTable>(this, TEXT("DataTable'/Game/InGame/Card/DT_CardDataTable.DT_CardDataTable'"));
	//if (CardDataTable)
	//{
	//	TArray<FName> RowNames = CardDataTable->GetRowNames();
	//	static const FString ContextString(TEXT("DataContext"));

	//	FCardData* CardData = CardDataTable->FindRow<FCardData>(CardKey, ContextString, true);
	//	if (CardData)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Bomb: Effect"));
	//		//폭탄에 색상 넣기
	//		CardData->CardType;

	//		//이후 로직
	//	}
	//}

	int32 Key = FCString::Atoi(*CardKey.ToString());
	int32 Color = Key / 100;
	int32 CardId = Key % 100;

	switch (Color)
	{
	case 1:
		//Red
		//색상 옵션~
		RedEffectLogic(CardId);
		break;

	case 2:
		//Blue
		BlueEffectLogic(CardId);
		break;

	case 3:
		//Yellow
		YellowEffectLogic(CardId);
		break;

	case 4:
		//Purple

		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("Bomb: ActivateCardEffect: Default"));
		break;
	}

	ABombGameModeBase* GM = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
	if (GM)
	{
		GM->ActiveEffectsFinished(Damage, Count);
	}
}

void ABomb::RedEffectLogic(int32 CardID)
{

	if (CardID < 6)	// Case of Swtich Damage 101~105
	{
		switch (CardID)
		{
		case 1:
			// 101 Add 1 Damage On Bomb
			Damage = FMath::Clamp(++Damage, 1, 100);
			break;

		case 2:
			// 102 Add 2 Damage On Bomb
			Damage = FMath::Clamp(Damage + 2, 1, 100);
			break;

		case 3:
			// 103 Remove 1 Damage On Bomb
			Damage = FMath::Clamp(--Damage, 1, 100);
			break;

		case 4:
			// 104 Remove 2 Damage On Bomb
			Damage = FMath::Clamp(Damage - 2, 1, 100);
			break;

		case 5:
			// 105 Add -5 ~ +5 Random Damage
			Damage = FMath::Clamp(Damage + FMath::RandRange(-5, 5), 1, 100);
			break;

		default:
			UE_LOG(LogTemp, Warning, TEXT("Bomb: RedEffectLogic: Default"));
			break;
		}
		EmissiveBombColor(FLinearColor::Red);
	}
	else if (CardID < 10)	//Case of Switch Count 106~109
	{
		switch (CardID)
		{
		case 6:
			// 106 Add 1 Count On Bomb
			Count = FMath::Clamp(++Count, 1, 100);
			break;

		case 7:
			// 107 Add 2 Count On Bomb
			Count = FMath::Clamp(Count + 2, 1, 100);
			break;

		case 8:
			//108 Remove 1 Count On Bomb
			Count = FMath::Clamp(--Count, 1, 100);
			break;

		case 9:
			// 109 Random count
			Count = FMath::Clamp(Count + FMath::RandRange(-3, 3), 1, 100);
			break;

		default:
			UE_LOG(LogTemp, Warning, TEXT("Bomb: RedEffectLogic: Default"));
			break;
		}
		EmissiveBombColor(FLinearColor::Red);
	}
	else //110, 111
	{
		if (CardID == 10)
		{
			//110 Visible Count
			//GM -> Visible(Count)
			EmissiveBombColor(FLinearColor::Red);
		}
		else
		{
			//Make Random
			//111 Bluffing
			//
			EmissiveBombColor(FLinearColor::Yellow);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Bomb: Red Effect Is Activated"));

	UE_LOG(LogTemp, Warning, TEXT("Bomb: %d Damage is Changed"), Damage);
	UE_LOG(LogTemp, Warning, TEXT("Bomb: %d Count is Chnaged"), Count);

	
}

void ABomb::BlueEffectLogic(int32 CardID)
{
	ABombGameModeBase* GM = GetWorld()->GetAuthGameMode<ABombGameModeBase>();
	if (GM)
	{
		switch (CardID)
		{
		case 1:
			// 201 Change Direction
			GM->Logic_ReverseDirection();
			break;

		case 2:
			// 202 Add Count On Bomb = Jump
			GM->Logic_JumpNextPlayer();
			break;

		case 3:
			// 203 Block Effect
			// GM-> Block()
			break;

		case 4:
			// 204 Eliminate yellow card
			// GM -> Eliminate()
			break;

		case 5:
			// 205 Make Next Player's Card 2
			// GM -> Make 2()
			break;

		case 6:
			// 206 Steal coin
			// GM -> Steal ();
			break;

		default:
			break;
		}
		EmissiveBombColor(FLinearColor::Blue);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Bomb: Blue Effect Is Activated"));
}

void ABomb::YellowEffectLogic(int32 CardID)
{
	switch (CardID)
	{
	case 1:
		// 301 Throw Bomb
		// GM ->
		break;

	case 2:
		// 302 Half Damage
		// GM -> 
		break;

	case 3:
		// 303 Die Together
		// GM-> 
		break;

	case 4:
		// 304 Make me die
		// GM ->
		break;

	case 5:
		// 305 change Damage to Coin
		// GM -> 
		break;

	case 6:
		// 306 
		// GM 
		break;

	default:
		break;
	}

	EmissiveBombColor(FLinearColor::Yellow);
	UE_LOG(LogTemp, Warning, TEXT("Bomb: Yellow Effect Is Activated"));
}

void ABomb::EmissiveBombColor(FLinearColor Color)
{
	UE_LOG(LogTemp, Warning, TEXT("Bomb: EmissiveBombColor"));
	if (EmissiveMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bomb: EmissiveBombColor Called"));

		EmissiveMaterial->SetVectorParameterValue("GlowColor", Color);

		GetWorld()->GetTimerManager().SetTimer(EmissiveHandle, this, &ABomb::StopEmissive, 1.f, false);
	}

}

void ABomb::StopEmissive()
{
	if (EmissiveMaterial)
	{
		EmissiveMaterial->SetVectorParameterValue("GlowColor", FLinearColor::Black);
	}
}



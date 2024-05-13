/*
* FileName	: AIC_RazorKiller.cpp
* Created	: Misora Tanaka
* Summary	: Program to control the Razor Killer AI
* Update	: 24/04/08 Create
*
*/

#include "AIC_RazorKiller.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"
#include "WarpPoint.h"
#include "UObject/ConstructorHelpers.h"

AAIC_RazorKiller::AAIC_RazorKiller(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	ConstructorHelpers::FObjectFinder<UBehaviorTree> const BTFinder(TEXT("/Game/Characters/RazorKiller/BT_RazorKiller.BT_RazorKiller"));
	BehaviorTree = BTFinder.Object;

	PlayerKeyName = "Player";
}


void AAIC_RazorKiller::BeginPlay()
{
	Super::BeginPlay();
}

void AAIC_RazorKiller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AIControllerがPawn所持した際にBBとBTを使用する
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BehaviorComp->StartTree(*BehaviorTree);
}

// 後で未来のわたしがTemplate関数にします1
void AAIC_RazorKiller::SetMoveArea() const
{
	TSubclassOf<ATriggerBox> FindClass = ATriggerBox::StaticClass();
	TArray<TObjectPtr<AActor>> Triggers;
	// これで全部のトリガーボックスを取得出来る
	// 距離を測る→一番近いやつをBBに代入(こいつは関数にして)
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindClass, Triggers);

	FVector OwnerPos = GetPawn()->GetActorLocation();
	float Difference = 0.f;
	float PreviousDifference = 100000000.f;
	int NearestNum = 0;
	for (int ti = 0; ti < Triggers.Num(); ti++)
	{
		FVector NowLocation = Triggers[ti]->GetActorLocation();
		Difference += UKismetMathLibrary::Abs(OwnerPos.X - NowLocation.X) + UKismetMathLibrary::Abs(OwnerPos.Y - NowLocation.Y) + UKismetMathLibrary::Abs(OwnerPos.Z - NowLocation.Z);
		if (Difference < PreviousDifference)
		{
			PreviousDifference = Difference;
			NearestNum = ti;
		}
	}
	if (Triggers[NearestNum] != nullptr)
	{
		BlackboardComp->SetValueAsObject("MoveBox", Triggers[NearestNum]);
	}
}

// 後で未来のわたしがTemplate関数にします2
void AAIC_RazorKiller::SetWarpPoint() const
{
	TSubclassOf<AWarpPoint> findClass;
	findClass = AWarpPoint::StaticClass();
	TArray<TObjectPtr<AActor>> WarpPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, WarpPoints);

	FVector OwnerPos = GetPawn()->GetActorLocation();
	float Difference = 0.f;
	float PreviousDifference = 10000000.f;
	int NearestNum = 0;
	for (int wi = 0; wi < WarpPoints.Num(); wi++)
	{
		FVector NowLocation = WarpPoints[wi]->GetActorLocation();
		Difference += UKismetMathLibrary::Abs(OwnerPos.X - NowLocation.X) + UKismetMathLibrary::Abs(OwnerPos.Y - NowLocation.Y) + UKismetMathLibrary::Abs(OwnerPos.Z - NowLocation.Z);
		if (Difference < PreviousDifference && WarpPoints[wi]->ActorHasTag(FName("Warp")))
		{
			PreviousDifference = Difference;
			NearestNum = wi;
		}
	}
	if (WarpPoints[NearestNum] != nullptr)
	{
		BlackboardComp->SetValueAsObject("WarpPoint", WarpPoints[NearestNum]);
	}
}

// 後で未来のわたしがTemplate関数にします3
void AAIC_RazorKiller::SetStartPoint() const
{
	TSubclassOf<AWarpPoint> const FindClass = AWarpPoint::StaticClass();
	TArray<TObjectPtr<AActor>> WarpPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindClass, WarpPoints);

	FVector const OwnerLocation = GetPawn()->GetActorLocation();
	float Difference = 0.f;
	float PreviousDifference = 10000000.f;
	int NearestNum = 0;
	for (int wi = 0; wi < WarpPoints.Num(); wi++)
	{
		FVector NowLocation = WarpPoints[wi]->GetActorLocation();
		Difference += UKismetMathLibrary::Abs(OwnerLocation.X - NowLocation.X) + UKismetMathLibrary::Abs(OwnerLocation.Y - NowLocation.Y) + UKismetMathLibrary::Abs(OwnerLocation.Z - NowLocation.Z);
		if (Difference < PreviousDifference && WarpPoints[wi]->ActorHasTag(FName("Start")))
		{
			PreviousDifference = Difference;
			NearestNum = wi;
		}
	}
	if (WarpPoints[NearestNum] != nullptr)
	{
		BlackboardComp->SetValueAsObject("StartPoint", WarpPoints[NearestNum]);
	}
}

void AAIC_RazorKiller::Summon() const
{
	BlackboardComp->SetValueAsBool("IsChasePart", true);
}



void AAIC_RazorKiller::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

TObjectPtr<APlayerCharacter> AAIC_RazorKiller::GetPlayerKey() const
{
	ensure(BlackboardComp);

	return Cast<APlayerCharacter>(BlackboardComp->GetValueAsObject(PlayerKeyName));
}


void AAIC_RazorKiller::SetPlayerKey(TObjectPtr<APawn> const Player) const
{
	// ブラックボードで作成したPlayerキーに情報を入れる
	BlackboardComp->SetValueAsObject(PlayerKeyName, Player);
}

void AAIC_RazorKiller::SetChaseKey() const
{
	// ブラックボードで作成したPlayerキーに情報を入れる
	BlackboardComp->SetValueAsBool("IsChaseTime", true);
}


void AAIC_RazorKiller::DeletePlayerKey() const
{
	BlackboardComp->ClearValue(PlayerKeyName);
}

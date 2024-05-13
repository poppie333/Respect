// Fill out your copyright notice in the Description page of Project Settings.

#include "RazorKiller.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.h"
#include "AIC_RazorKiller.h"

// Sets default values
ARazorKiller::ARazorKiller()
	: SeverityPoint(100.f)
	, VisionAngle(60.f)
	, ChaseTime(5.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComp"));
	// 視野
	SensingComp->SetPeripheralVisionAngle(VisionAngle);

	// 見える範囲
	SensingComp->SightRadius = 2000;
	SensingComp->OnSeePawn.AddDynamic(this, &ARazorKiller::OnSeePlayer);
}

// Called when the game starts or when spawned
void ARazorKiller::BeginPlay()
{
	Super::BeginPlay();
	AIRazorKiller = Cast<AAIC_RazorKiller>(GetController());
	if (AIRazorKiller)
	{
		AIRazorKiller->SetMoveArea();
		AIRazorKiller->SetWarpPoint();
		AIRazorKiller->SetStartPoint();
	}
	else
	{
		DebugLog("Error:RazorKiller's AIController is NULL");
	}
}

// Called every frame
void ARazorKiller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsChaseTime)
	{
		AIRazorKiller->SetChaseKey();
	}
}

void ARazorKiller::OnSeePlayer(APawn* Pawn)
{
	//プレイヤー
	TObjectPtr<APlayerCharacter> const Player = Cast<APlayerCharacter>(Pawn);
	
	if (AIRazorKiller && Player)
	{
		// AIControllerにプレイヤー情報を設定
		AIRazorKiller->SetPlayerKey(Player);
	}
	
	if (ChaseHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ChaseHandle);
	}
	else
	{
		GetWorldTimerManager().SetTimer(ChaseHandle, this, &ARazorKiller::LostSeePlayer, ChaseTime, false);
	}

	DebugPringString("See", FColor::Blue, 2.f);
}

void ARazorKiller::LostSeePlayer() const
{
	if (AIRazorKiller)
	{
		AIRazorKiller->DeletePlayerKey();
	}

	DebugPringString("Lost", FColor::Red, 2.f);
}

void ARazorKiller::DebugPringString(const FString Log, const FColor Color, const float PrintTime) const
{
	if (!bShowLog)
	{
		return;
	}
	UKismetSystemLibrary::PrintString(this, Log, true, true, Color, PrintTime);
}

void ARazorKiller::DebugLog(FString const Log) const
{
	if (!bShowLog)
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("%s"), *Log);
}

﻿///--------------------------------------------------------------------------------------
// ファイル名	：CorridorCamera.cpp
// 内容			：廊下用のカメラ作成
// 作成者		：22cu0236 八町陸斗
// 更新日		：2024/04/11		作成
//--------------------------------------------------------------------------------------

#include "CorridorCamera.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ACorridorCamera::ACorridorCamera()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void ACorridorCamera::BeginPlay()
{
	Super::BeginPlay();

	// APawn の取得
	APawn* aPlayerPawn = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
	TragetCharacter = (AActor*)aPlayerPawn;

	// 初期位置の設定
	startLocation = this->GetActorLocation();

}

// Called every frame
void ACorridorCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Corridor Camera の位置をターゲットキャラクターに合わせる
	FVector location = FVector(this->GetActorLocation().X, TragetCharacter->GetActorLocation().Y, this->GetActorLocation().Z);

	// 位置を設定
	this->SetActorLocation(location);

}
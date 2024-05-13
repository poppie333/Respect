// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraChangeArea.h"
#include "Components/BoxComponent.h"
#include "CameraManager.h"
#include "PlayerCharacter.h"

ACameraChangeArea::ACameraChangeArea()
{
    // ボックスコンポーネントのデフォルトを作成
    OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraProximityVolume"));
    // ボックスコンポネントをルートコンポネントに取り付ける
    OverlapVolume->SetupAttachment(RootComponent);
}

// オーバーラップ開始
void ACameraChangeArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
    // オーバーラップしているアクタがプレイヤーキャラクターかどうかのキャストチェック
    if (APlayerCharacter* PlayerCharacterCheck = Cast<APlayerCharacter>(OtherActor))
    {
        // プレイヤー キャラクターの PlayerController へのキャスト
        if (APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerCharacterCheck->GetController()))
        {
            //見つかった最初の CameraActor にプレイヤー コントローラーのビューを設定します
            PlayerCharacterController->SetViewTargetWithBlend(CameraActor, 0.0f, EViewTargetBlendFunction::VTBlend_Linear);
        }
    }
}
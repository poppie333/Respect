// Fill out your copyright notice in the Description page of Project Settings.


#include "Closet.h"

void ACloset::BeginPlay()
{
    Super::BeginPlay();
    USceneComponent* RotationRootComponent = GetRotationRoot();
    if (IsValid(RotationRootComponent))
    {
        ClosedDoorYawRotation = RotationRootComponent->GetRelativeRotation().Yaw;
        OpenedDoorYawRotation = ClosedDoorYawRotation + OpenAngle;
    }
    SetActorTickEnabled(false);
}

void ACloset::Tick(float DeltaTime)
{
    //ドアが開閉動作中か？
    if ((MoveDir == 1) || (MoveDir == -1))
    {
        //回転の支点となるシーンコンポーネントの取得
        USceneComponent* RotationRootComponent = GetRotationRoot();

        //カーブアセットと支点コンポーネントは有効か？
        if (IsValid(MovementCurve) && IsValid(RotationRootComponent))
        {
            //進行時間を更新する
            ElapsedTime += DeltaTime * MoveDir;

            //Curveの進行時間の最小値と最大値を取得
            float MinCurveTime, MaxCurveTime;
            MovementCurve->GetTimeRange(MinCurveTime, MaxCurveTime);

            //進行時間の範囲を超えないように丸め込み
            ElapsedTime = FMath::Clamp(ElapsedTime, MinCurveTime, MaxCurveTime);

            //閉じてる時から開いてる時までの割合(0.0~1.0)を取得
            float MoveAlpha = MovementCurve->GetFloatValue(ElapsedTime);

            //座標の更新
            FRotator NewRotation = RotationRootComponent->GetRelativeRotation();
            NewRotation.Yaw = FMath::Lerp(ClosedDoorYawRotation, OpenedDoorYawRotation, MoveAlpha);
            RotationRootComponent->SetRelativeRotation(NewRotation);

            //進行時間がCurveの最大値を超えたらTickを停止する
            if (ElapsedTime >= MaxCurveTime)
            {
                MoveDir = -1;
                SetPlayerLocation();
            }
            else if (ElapsedTime <= MinCurveTime)
            {
                if (MoveDir == -1)
                {
                    //UE_LOG(LogTemp, Warning, TEXT("最小値"));
                    MoveDir = 0;
                    SetActorTickEnabled(false);
                }
            }
        }
    }
}

void ACloset::Interactive()
{
    //移動フラグとTick処理を有効にする
    MoveDir = 1;
    SetActorTickEnabled(true);
}

E_PlayerState ACloset::GetState()
{
    return E_PlayerState::InCloset;
}

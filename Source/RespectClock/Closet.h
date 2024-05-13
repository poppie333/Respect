// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HideObject.h"
#include "Closet.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API ACloset : public AHideObject
{
	GENERATED_BODY()
	
protected:
    void BeginPlay();

public:
    // Called every frame
    void Tick(float DeltaTime) override;

    void Interactive() override;

    virtual E_PlayerState GetState();

    UFUNCTION(BlueprintImplementableEvent)
        USceneComponent* GetRotationRoot();

public:
    //開いた時の角度
    UPROPERTY(EditDefaultsOnly)
        float OpenAngle = 120.0f;

    //開閉の仕方を指定するカーブアセット
    UPROPERTY(EditDefaultsOnly)
        UCurveFloat* MovementCurve;

private:
    //開閉処理の進行時間
    float ElapsedTime;
    //開閉方向　1=開く　0=何もしない　-1=閉じる
    int MoveDir = 0;
    //ドアが閉まっているときのアクターのローテーション
    float ClosedDoorYawRotation;
    //ドアが開ききっているのアクターのローテーション
    float OpenedDoorYawRotation;

};

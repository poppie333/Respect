//--------------------------------------------------------------------------------------
// ファイル名：GM_RespectClock.h
// 内容		：ゲームのルールを制御する
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/23		作成
//			：2024/04/30		ビューポートの追加、削除処理、レベルのリスタート処理(後に決められた地点へのリスポーン処理に変更)、リスポーン地点設定処理を記載
//--------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_RespectClock.generated.h"

//前方宣言
class UUserWidget;

UCLASS()
class RESPECTCLOCK_API AGM_RespectClock : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//コンストラクタ
	AGM_RespectClock();
	//開始時の処理
	void BeginPlay()override;

private:
	//Viewport追加
	UFUNCTION(BlueprintCallable, Category = "RespectClock")
	UUserWidget* View(UUserWidget* _widget);
	//ViewPort削除
	UFUNCTION(BlueprintCallable, Category = "RespectClock")
	void Remove();
	//リスポーン
	UFUNCTION(BlueprintCallable, Category = "RespectClock")
	void Respawn();

private:
	//リスポーン座標
	UPROPERTY()
	FVector m_RespawnPos;
	UPROPERTY()
	UUserWidget* m_pGameCOWidget;

public:
	void SetRespawnPos(FVector _respawnPos);

// イベントディスパッチャー
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameRuleDispatcer);
public:
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FGameRuleDispatcer GameClearDispatcer;


};

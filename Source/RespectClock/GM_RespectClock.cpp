//--------------------------------------------------------------------------------------
// ファイル名：GM_RespectClock.cpp
// 内容		：ゲームのルールを制御する
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/23		作成、関数宣言
//			：2024/04/30		ビューポートの追加、削除処理、レベルのリスタート処理(後に決められた地点へのリスポーン処理に変更)、リスポーン地点設定処理を記載
//--------------------------------------------------------------------------------------

#include "GM_RespectClock.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

//コンストラクタ
AGM_RespectClock::AGM_RespectClock()
	: m_RespawnPos(FVector::ZeroVector)
	, m_pGameCOWidget(nullptr)
{

}

//開始時処理
void AGM_RespectClock::BeginPlay()
{
	Super::BeginPlay();

	//スタート時点での座標を取得

}

//Viewport追加
UUserWidget* AGM_RespectClock::View(UUserWidget* _Widget)
{
	//変数に入れる
	if (m_pGameCOWidget == nullptr)
	{
		m_pGameCOWidget = _Widget;
	}
	else
	{
		m_pGameCOWidget = nullptr;
		m_pGameCOWidget = _Widget;
	}

	if (m_pGameCOWidget)
	{
		// Viewportに追加する
		m_pGameCOWidget->AddToViewport(0);
	}

	return m_pGameCOWidget;
}

void AGM_RespectClock::Respawn()
{
	//レベルを再ロード(Prototype的実装)
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	//リスポーン地点へ移動(ある程度のstatusをリセットするかも？)

}

//Viewport削除
void AGM_RespectClock::Remove()
{
	if (m_pGameCOWidget)
	{
		//ViewPortから削除
		m_pGameCOWidget->RemoveFromParent();
		//変数を初期化
		m_pGameCOWidget = nullptr;
	}
}

//再スタート地点を設定する関数
void AGM_RespectClock::SetRespawnPos(FVector _respawnPos)
{
	m_RespawnPos = _respawnPos;
}

//
// 　ここでやること
// ---ゲームオーバー---
// 　 Widget表示(GameOver)
//	  リスポーン
//
// ---ゲームクリアー---
//	  Widget表示(GameClear)
//	  エンド分岐のクリア判定(どのエンドをクリアしたか)
// 　 レベルの再ロード(Title)
// 
// -----インゲーム-----
//    エンド分岐のフラグ管理
// 　 リスポーン位置の確保、管理
//	　
//
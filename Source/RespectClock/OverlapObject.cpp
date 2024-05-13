//--------------------------------------------------------------------------------------
// ファイル名：GM_RespectClock.cpp
// 内容		：ゲームのルールを制御する
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/23		作成、関数宣言
//			：2024/04/30		ビューポートの追加、削除処理、レベルのリスタート処理(後に決められた地点へのリスポーン処理に変更)、リスポーン地点設定処理を記載
//--------------------------------------------------------------------------------------

#include "OverlapObject.h"
#include "Components/BoxComponent.h"

// Sets default values
AOverlapObject::AOverlapObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ボックスコンポーネント生成
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (m_pBoxComp)
	{
		RootComponent = m_pBoxComp;
	}
}

// Called when the game starts or when spawned
void AOverlapObject::BeginPlay()
{
	Super::BeginPlay();
	
	//オーバーラップ接触し始めたときに呼ばれる処理
	if (m_pBoxComp != NULL)
	{
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AOverlapObject::OnOverlapBegin);
	}
}

void AOverlapObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
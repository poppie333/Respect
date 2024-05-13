//--------------------------------------------------------------------------------------
// ファイル名：BTT_MoveToPlayer.h
// 内容		：犬の動きを制御するAIコントローラー
// 作成者	：22cu0127 丹代天
// 更新日	：2024/04/16		作成、プレイヤーへ近づく処理記述
//--------------------------------------------------------------------------------------

#include "BTT_MoveToPlayer.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "VisualLogger/VisualLogger.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//コンストラクタ
UBTT_MoveToPlayer::UBTT_MoveToPlayer()
{
	//ビヘイビアツリーノードの名前を記述
	NodeName = TEXT("MoveToPlayer");
	//Tickを回すかどうか判断
	bNotifyTick = true;
	//WorldContextから、現在使用しているナビゲーションシステムを取得
	navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	//ブラックボードキーにフィルターをかける
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MoveToPlayer, BlackboardKey), APawn::StaticClass());
}

//タスク開始時に呼ばれる処理
EBTNodeResult::Type UBTT_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	// Targetを取得
	UBlackboardComponent* pBlackboard = _ownerComp.GetBlackboardComponent();
	pTarget = Cast<APawn>(pBlackboard->GetValueAsObject(TEXT("PlayerActor")));

	// Tickで移動処理するのでInProgressを返す
	return EBTNodeResult::InProgress;
}

//フレームごとに呼ばれる処理
void UBTT_MoveToPlayer::TickTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSeconds)
{
	//移動処理
	UpdateMove(_ownerComp, _deltaSeconds);
}

//移動処理
void UBTT_MoveToPlayer::UpdateMove(UBehaviorTreeComponent& _ownerComp, float _deltaTime)
{
	//AIコントローラー取得
	AAIController* pController = _ownerComp.GetAIOwner();
	//コントローラーが保持している、Pawnを取得
	APawn* pOwner = pController->GetPawn();

	//目標までの距離を格納する変数
	FVector distance;

	// ターゲットに到達しているのかの判定
	//Pawnとコントローラーが存在している時
	if (pOwner && pTarget)
	{
		//distanceにターゲットまでの距離を格納する
		distance = pOwner->GetActorLocation() - pTarget->GetActorLocation();
	}

	//対象に向かって動く際に、どこまで近づくかを計算する
	//移動距離が半径よりも長かった時
	if (distance.SquaredLength() > acceptableRadius * acceptableRadius)
	{
		//位置フレーム間の移動距離を計算
		FVector movement = CalcMovement(pOwner, _deltaTime);

		//移動距離がゼロでなかった場合
		if (!movement.IsZero())
		{
			//移動
			pOwner->AddMovementInput(movement);
		}
	}
	//移動距離が近づける距離よりも短かった時
	else
	{
		// タスクを終了させる
		FinishLatentTask(_ownerComp, EBTNodeResult::Succeeded);
	}

	return;
}

//移動経路の計算
FVector UBTT_MoveToPlayer::CalcMovement(const APawn* _pOwner, float _deltaTime)
{
	// NavigationMeshを用いて移動経路を探索
	UNavigationPath* pPath = navSys->FindPathToActorSynchronously(GetWorld(), _pOwner->GetActorLocation(), pTarget);
	//パスが見つからなかった場合、処理を返す
	if (!pPath) { return FVector::Zero(); }

	//Pathpointの値を配列に格納する	
	TArray<FVector> pathPoints = pPath->PathPoints;

	//pathpointが二つ以上の場合(移動する必要がある場合)
	if (pathPoints.Num() >= 2)
	{
		// 自身の座標から初めの地点への方向を返す
		FVector dir = pathPoints[1] - pathPoints[0];
		dir.Normalize();

		//デバッグ用
		//pPathがNULLでない場合
		if (pPath)
		{
			for (int pointiter = 0; pointiter < pPath->PathPoints.Num(); pointiter++)
			{
				//パスポイントに球を描画する
				DrawDebugSphere(GetWorld(), pPath->PathPoints[pointiter], 10.0f, 12, FColor(255, 0, 0));
			}
		}

		//移動方向をもとにMeshを回転させる
		UpdateRotate(dir, _pOwner, _deltaTime);

		return dir;
	}
	else
	{
		return FVector::Zero();
	}
}

//Meshの回転処理
void UBTT_MoveToPlayer::UpdateRotate(const FVector _direction, const APawn* _pOwner, float _deltaTime)
{
	//プレイヤーのメッシュ取得
	USkeletalMeshComponent* pMeshComp = Cast<ACharacter>(_pOwner)->GetMesh();
	//角度はDegreeの角度の範囲で表記(-180°～180°)
	if (pMeshComp != NULL)
	{
		//アークタンジェントを使ってコントローラーの入力方向がなす角度を求める
		float angle = atan2(-_direction.X, _direction.Y);
		//Radian値をDegreeに変換
		float angleDeg = FMath::RadiansToDegrees(angle);

		//回転を設定する
		pMeshComp->SetRelativeRotation(FRotator(pMeshComp->GetRelativeRotation().Pitch, angleDeg, pMeshComp->GetRelativeRotation().Roll));
	}
}
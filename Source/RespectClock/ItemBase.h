// ItemBase.h

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "ItemData.h"
#include "ItemBase.generated.h"

class APlayerCharacter;

UCLASS()
class RESPECTCLOCK_API AItemBase : public AInteractiveObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	void BeginPlay();

	// インタラクトされたときに呼ばれる関数
	void Interactive() override;

	// アイテムがインベントリに追加されたときの処理
	// 取ったらイベントやアイテムの削除などを行う
	virtual void OnAddedToInventory();

	APlayerCharacter* Player;	// プレイヤーを取得するため
	FTimerHandle ItemTimer;

public:	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;	// 所持しているアイテムのデータ

	// アイテムを取ったときのアニメーション、座標、回転
	virtual E_PlayerState GetState() override;
	virtual FRotator GetLerpPointRot() override;
	virtual FVector GetLerpPointPos() override;

	int32 GetID() const { return ItemData.ID; }	// ID取得
};

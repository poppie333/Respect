// ItemBase.cpp

#include "ItemBase.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItemBase::AItemBase()
{

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
    Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// インタラクトされたらアイテムをインベントリに追加
void AItemBase::Interactive()
{
    if (Player != NULL)
    {
        Player->AddItemToInventory(this);
        Player->BPKillerSummon();
        OnAddedToInventory();
    }   
}

// アイテムがインベントリに追加されたときの処理
void AItemBase::OnAddedToInventory()
{

    SetActorHiddenInGame(true); // アイテムを非表示
    SetActorEnableCollision(false); // コリジョン無効
    Destroy();  // 破棄
}

E_PlayerState AItemBase::GetState()
{
    return E_PlayerState::Pick;
}
FRotator AItemBase::GetLerpPointRot()
{
    return Player->GetActorRotation();
}
FVector AItemBase::GetLerpPointPos()
{
    return Player->GetActorLocation();
}
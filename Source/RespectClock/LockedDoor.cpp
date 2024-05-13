// Fill out your copyright notice in the Description page of Project Settings.

#include "LockedDoor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "ItemBase.h"

void ALockedDoor::Interactive()
{
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	TArray<AItemBase*> PlayerInventory = Player->GetInventory();
	if (Player != NULL)
	{
		for (AItemBase* Item : PlayerInventory)
		{
			// アイテムがドアのIDと一致するかどうかをチェックする条件を追加する
			if (Item->GetID() == DoorID)
			{
				CanOpen = true;
				break;	// 見つけた時点で終了
			}
		}
	}
	if (CanOpen != false)
		Super::Interactive();
}
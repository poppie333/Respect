// ItemData.h
// アイテムのデータの構造体

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 追加した変数
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item")
	FName Name;	// アイテム名

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item")
	int ID;	// ID

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Texture; // アイテム欄で表示するテクスチャ 

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item")
	FText Description;	// アイテムの説明

};
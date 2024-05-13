// PlayerInventory.h

// 今使い物になりません

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.h"
#include "PlayerInventory.generated.h"

UCLASS()
class RESPECTCLOCK_API UPlayerInventory : public UObject
{
    GENERATED_BODY()
    
public:
    UPlayerInventory();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<AItemBase*> InventoryItem;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItem(AItemBase* Item);
};
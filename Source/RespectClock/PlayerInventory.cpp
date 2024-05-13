// PlayerInventory.cpp

// 今使い物になりません

#include "PlayerInventory.h"

UPlayerInventory::UPlayerInventory()
{

}

void UPlayerInventory::AddItem(AItemBase* Item)
{
    if (Item)
    {
        InventoryItem.Add(Item);

        UE_LOG(LogTemp, Display, TEXT("Item added to inventory: %s"), *Item->GetName());
    }
    else if (!Item)
    {
        UE_LOG(LogTemp, Display, TEXT("Item Is NULL."));
    }
}
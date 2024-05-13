// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "LockedDoor.generated.h"

class APlayerCharacter;

UCLASS()
class RESPECTCLOCK_API ALockedDoor : public ADoor
{
	GENERATED_BODY()
	
private:
	APlayerCharacter* Player;
	bool CanOpen;	// ドアは開閉可能か
public:
	void Interactive() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID");
	int32 DoorID;	// ドアID
};

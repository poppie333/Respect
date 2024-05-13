// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObject.h"
#include "HideObject.generated.h"

/**
 * 
 */
UCLASS()
class RESPECTCLOCK_API AHideObject : public AInteractiveObject
{
	GENERATED_BODY()

public:
	AHideObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetHidePos();
	FRotator GetHideRot();

	virtual E_PlayerState GetState() override;

protected:
	void SetPlayerLocation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> HidePointScene;
};

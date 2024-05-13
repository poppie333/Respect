// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractiveObject.generated.h"

class USphereComponent;
class USceneComponent;

UCLASS()
class RESPECTCLOCK_API AInteractiveObject : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetLerpPointPos();
	FRotator GetLerpPointRot();

	virtual void Interactive() override;

	virtual FVector GetHidePos()override;
	virtual FRotator GetHideRot()override;

	virtual E_PlayerState GetState() override;

protected:
	// 当たり判定スフィア
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Sphere;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultScene;
	// インタラクティブした時に線形補間するポイント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> LerpPointScene;
};

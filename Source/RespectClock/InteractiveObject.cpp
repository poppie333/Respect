// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "PlayerCharacter.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultScene;

	LerpPointScene = CreateDefaultSubobject<USceneComponent>(TEXT("LerpPoint"));
	LerpPointScene->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AInteractiveObject::GetLerpPointPos()
{
	if (!IsValid(LerpPointScene)) return FVector::ZeroVector;
	return LerpPointScene->GetComponentLocation();
}

FRotator AInteractiveObject::GetLerpPointRot()
{
	if (!IsValid(LerpPointScene)) return FRotator::ZeroRotator;
	FRotator rot = LerpPointScene->GetComponentRotation();
	// プレイヤー回ってるから補正
	rot.Yaw -= 90.0f;
	return rot;
}

void AInteractiveObject::Interactive()
{
	// アイテムとのインタラクト処理
	UE_LOG(LogTemp, Warning, TEXT("papadayo"));
}

FVector AInteractiveObject::GetHidePos()
{
	return FVector();
}

FRotator AInteractiveObject::GetHideRot()
{
	return FRotator();
}

E_PlayerState AInteractiveObject::GetState()
{
	return E_PlayerState();
}


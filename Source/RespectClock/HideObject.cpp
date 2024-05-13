// Fill out your copyright notice in the Description page of Project Settings.


#include "HideObject.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"

AHideObject::AHideObject()
{
	HidePointScene = CreateDefaultSubobject<USceneComponent>(TEXT("HidePoint"));
	HidePointScene->SetupAttachment(RootComponent);
}

void AHideObject::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AHideObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AHideObject::GetHidePos()
{
	if (!IsValid(HidePointScene)) return FVector::ZeroVector;
	return HidePointScene->GetComponentLocation();
}

FRotator AHideObject::GetHideRot()
{
	if (!IsValid(HidePointScene)) return FRotator::ZeroRotator;
	FRotator rot = HidePointScene->GetComponentRotation();
	rot.Yaw += 90.0f;
	return rot;
}

E_PlayerState AHideObject::GetState()
{
	return E_PlayerState();
}

void AHideObject::SetPlayerLocation()
{
	TArray<AActor*> Actors;
	APlayerCharacter* Player = nullptr;
	// OverlapされているActorを返す
	Sphere->GetOverlappingActors(Actors);


	for (AActor* actor : Actors)
	{
		Player = Cast<APlayerCharacter>(actor);
		// もしインタラクトできるなら
		if (Player != nullptr)
		{
			if (Player->IsHiding)
			{
				Player->SetActorLocation(FVector(GetLerpPointPos().X, GetLerpPointPos().Y, Player->GetActorLocation().Z));
				Player->IsMoveable = true;
				Player->IsHiding = false;
				Player->PlayerStatus = E_PlayerState::Idle;
			}
		}
	}
}
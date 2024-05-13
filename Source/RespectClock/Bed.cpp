// Fill out your copyright notice in the Description page of Project Settings.


#include "Bed.h"

ABed::ABed()
	:IsHide(false)
{

}

void ABed::BeginPlay()
{
	SetActorTickEnabled(false);
}

void ABed::Tick(float DeltaTime)
{

}

void ABed::Interactive()
{
	if (!IsHide)
	{
		UE_LOG(LogTemp, Warning, TEXT("true"));
		IsHide = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("false"));
		SetPlayerLocation();
		IsHide = false;
	}
}

E_PlayerState ABed::GetState()
{
	return E_PlayerState::Crawl;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "E_PlayerState.generated.h"

UENUM(BlueprintType)
enum class E_PlayerState : uint8
{
	Idle,
	Walk,
	Run,
	Sit,
	Fall,
	Pick,
	Open,
	Lock,
	InCloset,
	Crawl,
};

UENUM(BlueprintType)
enum class E_PlayerStaminaState : uint8
{
	Stamina_100,
	Stamina_75,
	Stamina_50,
	Stamina_25,
	Stamina_0
};

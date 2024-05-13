// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveInput.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMoveInput::UMoveInput()
	:IsMoving(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMoveInput::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMoveInput::Move(FVector2D invalue)
{
	IsMoving = true;

	// cast player
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

	// get active camera
	APlayerCameraManager* aPlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	// get forward direction
	FVector ForwardDirection;
	if (aPlayerCameraManager->GetCameraRotation().Pitch > 45.0f || aPlayerCameraManager->GetCameraRotation().Pitch < -45.0f)
	{
		ForwardDirection = aPlayerCameraManager->GetActorUpVector();
	}
	else
	{
		ForwardDirection = aPlayerCameraManager->GetActorForwardVector();
	}
	// get right direction
	const FVector RightDirection = aPlayerCameraManager->GetActorRightVector();
	
	// add movement
	Player->AddMovementInput(ForwardDirection, invalue.Y);
	Player->AddMovementInput(RightDirection, invalue.X);

	if (Player->PlayerStatus != E_PlayerState::Run)
	{
		Player->PlayerStatus = E_PlayerState::Walk;
	}

	bool IsWalk = Player->PlayerStaminaPercentage == E_PlayerStaminaState::Stamina_100 || Player->PlayerStaminaPercentage == E_PlayerStaminaState::Stamina_75;
	if(IsWalk) Player->GetCharacterMovement()->MaxWalkSpeed = Player->PlayerParam.WalkSpeed;

}

// 走りの段階分け
void UMoveInput::Run()
{
	if (IsMoving)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

		if (Player->PlayerParam.Stamina > 0) Player->PlayerParam.Stamina -= Player->PlayerParam.StaminaDecreasedValue;

		switch (Player->PlayerStaminaPercentage)
		{
		case E_PlayerStaminaState::Stamina_100:
			Player->GetCharacterMovement()->MaxWalkSpeed = Player->PlayerParam.RunSpeed_100;
			Player->PlayerStatus = E_PlayerState::Run;
			break;

		case E_PlayerStaminaState::Stamina_75:
			Player->GetCharacterMovement()->MaxWalkSpeed = Player->PlayerParam.RunSpeed_75;
			Player->PlayerStatus = E_PlayerState::Run;
			break;

		case E_PlayerStaminaState::Stamina_50:
			Player->GetCharacterMovement()->MaxWalkSpeed = Player->PlayerParam.RunSpeed_50;
			Player->PlayerStatus = E_PlayerState::Run;
			break;

		case E_PlayerStaminaState::Stamina_25:
			Player->GetCharacterMovement()->MaxWalkSpeed = Player->PlayerParam.RunSpeed_25;
			Player->PlayerStatus = E_PlayerState::Run;
			break;

		case E_PlayerStaminaState::Stamina_0:
			Player->PlayerStatus = E_PlayerState::Fall;
			Player->IsMoveable = false;
			Player->PlayerParam.Stamina += 50.0f;
			break;
		}
	}
}

void UMoveInput::Sit()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player->IsMoveable)
	{
		Player->IsMoveable = false;
		Player->PlayerStatus = E_PlayerState::Sit;
	}
	else
	{
		Player->PlayerStatus = E_PlayerState::Idle;
		Player->IsMoveable = true;
	}
}

// Called every frame
void UMoveInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


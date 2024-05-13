// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveInput.h"
#include "PlayerCharacter.h"
#include "InteractableInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UInteractiveInput::UInteractiveInput()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveInput::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractiveInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractiveInput::Interactive()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());

	TArray<AActor*> Actors;
	IInteractableInterface* InteractableActor = nullptr;
	// OverlapされているActorを返す
	Player->GetOverlappingActors(Actors);

	for (AActor* actor : Actors)
	{
		InteractableActor = Cast<IInteractableInterface>(actor);
		// もしインタラクトできるなら
		if (InteractableActor != nullptr)
		{
			if (Player->PlayerStatus != E_PlayerState::Sit)
			{
				if (!Player->IsHiding && Player->IsMoveable)
				{
					UE_LOG(LogTemplateCharacter, Error, TEXT("ハイド三毛"), *GetNameSafe(this));
					Player->IsMoveable = false;
					Player->PlayerStatus = InteractableActor->GetState();
					Player->SetActorLocation(FVector(InteractableActor->GetLerpPointPos().X, InteractableActor->GetLerpPointPos().Y, Player->GetActorLocation().Z));
					Player->SetActorRotation(InteractableActor->GetLerpPointRot());
					if (actor->ActorHasTag("HideObject"))
					{
						//Player->GetCapsuleComponent()->SetCapsuleHalfHeight(1.0f);
						Player->HidePos = InteractableActor->GetHidePos();
						Player->HideRot = InteractableActor->GetHideRot();
					}
				}
				InteractableActor->Interactive();
			}
		}
	}
}


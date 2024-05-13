//-------------------------------------------------------------------------------------
// ファイル名：PlayerCharacter.cpp
// 処理内容	：Playerの挙動を制御をするキャラクタークラス
// 作成者	：日向野大聖
// 作成日	：04/08
// 更新日	：04/08		プレイヤーの移動処理追加
//			　
//-------------------------------------------------------------------------------------


#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MoveInput.h"
#include "InteractiveInput.h"
#include "ItemBase.h"	// 05/10追加
//-----22cu0127 丹代天 2024/05/07 追記：カプセルコンポーネント、レイザーキラーインクルード-----
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "RazorKiller.h"
//-----------------------------------------------------------------------------

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATestClockTowerCharacter

APlayerCharacter::APlayerCharacter()
	: DefaultMappingContext(nullptr)
	, MoveAction(nullptr)
	, RunAction(nullptr)
	, SitAction(nullptr)
	, InteractiveAction(nullptr)
	, DogAction(nullptr)
	, ItemAction(nullptr)
	, InputValueMin(0.7f)
	, PlayerParam(300.f, 500.f, 375.f, 250.f, 125.f, 100.f, 0.16f, 0.08f, 0.04f, 0.16f)
	, IsMoveable(true)
	, PlayerStatus(E_PlayerState::Idle)
	, PlayerStaminaPercentage(E_PlayerStaminaState::Stamina_100)
	, HidePos(FVector::ZeroVector)
	, HideRot(FRotator::ZeroRotator)
	, IsHiding(false)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = PlayerParam.WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	MoveInput = CreateDefaultSubobject<UMoveInput>(TEXT("MoveInputComponent"));
	InteractiveInput = CreateDefaultSubobject<UInteractiveInput>(TEXT("Interactive"));

	//-----22cu0127 丹代天 2024/05/07 追記：ヒットイベント関数登録-----
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy"));
	if (m_pBoxComp)
	{
		m_pBoxComp->SetupAttachment(RootComponent);
	}
	//-----------------------------------------------------------------
}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (m_pBoxComp)
	{
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnHitActor);
	}
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecoverStamina();
}

// 入力
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// バインドアクションの設定
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopMove);
		// Run Press
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Run);
		// Run Release
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopRun);
		// Sit
		EnhancedInputComponent->BindAction(SitAction, ETriggerEvent::Started, this, &APlayerCharacter::Sit);
		// Action
		EnhancedInputComponent->BindAction(InteractiveAction, ETriggerEvent::Started, this, &APlayerCharacter::Interactive);
		// Dog
		EnhancedInputComponent->BindAction(DogAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dog);
		// Item
		EnhancedInputComponent->BindAction(ItemAction, ETriggerEvent::Started, this, &APlayerCharacter::ItemSlect);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (IsMoveable)
		{
			MoveInput->Move(MovementVector);
			//UE_LOG(LogTemplateCharacter, Error, TEXT("Walk!"), *GetNameSafe(this));
		}
	}
}

void APlayerCharacter::StopMove()
{
	if (IsMoveable)
	{
		MoveInput->IsMoving = false;
		PlayerStatus = E_PlayerState::Idle;
	}
}

void APlayerCharacter::Run()
{
	if (IsMoveable)
	{
		MoveInput->Run();
		//UE_LOG(LogTemplateCharacter, Error, TEXT("%d"), (int)PlayerParam.Stamina);
	}
}

void APlayerCharacter::StopRun()
{
	if (IsMoveable)
	{
		if (GetCharacterMovement()->MaxWalkSpeed > PlayerParam.WalkSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = PlayerParam.WalkSpeed;
		}
		PlayerStatus = E_PlayerState::Idle;
		//UE_LOG(LogTemplateCharacter, Error, TEXT("Idele"));
	}
}

void APlayerCharacter::Sit()
{
	MoveInput->Sit();
	//UE_LOG(LogTemplateCharacter, Error, TEXT("Sit!"), *GetNameSafe(this));
}

void APlayerCharacter::Interactive()
{
	InteractiveInput->Interactive();
}

void APlayerCharacter::Dog(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D InputVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 下方向入力
		if(InputVector.Y > InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("した"), *GetNameSafe(this));
		}
		// 上方向入力
		else if(InputVector.Y < -InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("うえ"), *GetNameSafe(this));
		}
		// 右方向入力
		else if(InputVector.X > InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("みぎ"), *GetNameSafe(this));
		}
		// 左方向入力
		else if(InputVector.X < -InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("ひだり"), *GetNameSafe(this));
		}
	}
}

void APlayerCharacter::ItemSlect(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D InputVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 下方向入力
		if (InputVector.Y > InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("した"), *GetNameSafe(this));
		}
		// 上方向入力
		else if (InputVector.Y < -InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("うえ"), *GetNameSafe(this));
		}
		// 右方向入力
		else if (InputVector.X > InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("みぎ"), *GetNameSafe(this));
		}
		// 左方向入力
		else if (InputVector.X < -InputValueMin)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("ひだり"), *GetNameSafe(this));
		}
	}
}

void APlayerCharacter::RecoverStamina()
{
	if (PlayerParam.Stamina < 100)
	{
		switch (PlayerStatus)
		{
		case E_PlayerState::Idle:
			PlayerParam.Stamina += PlayerParam.StaminaRecoverdValue_Idle;
			break;
		case E_PlayerState::Walk:
			PlayerParam.Stamina += PlayerParam.StaminaRecoverdValue_Walk;
			break;
		case E_PlayerState::Sit:
			PlayerParam.Stamina += PlayerParam.StaminaRecoverdValue_Sit;
			break;
		default:
			break;
		}

		//UE_LOG(LogTemplateCharacter, Error, TEXT("%d"), (int)PlayerParam.Stamina);

		if (PlayerParam.Stamina > 75.0f)PlayerStaminaPercentage = E_PlayerStaminaState::Stamina_100;
		else if (PlayerParam.Stamina > 50.0f)PlayerStaminaPercentage = E_PlayerStaminaState::Stamina_75;
		else if (PlayerParam.Stamina > 25.0f)PlayerStaminaPercentage = E_PlayerStaminaState::Stamina_50;
		else if (PlayerParam.Stamina > 0.0f)PlayerStaminaPercentage = E_PlayerStaminaState::Stamina_25;
		else PlayerStaminaPercentage = E_PlayerStaminaState::Stamina_0;
	}
}

void APlayerCharacter::Hide()
{
	SetActorLocation(HidePos);
	SetActorRotation(HideRot);
}

//-----22cu0127 丹代天 2024/05/07 追記：関数定義-----
// 衝突判定
void APlayerCharacter::OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("collision"));

	if (IsEnemy(OtherActor) && m_bCollisionCheck)
	{
		//これを呼べばイベントに登録された処理が呼び出せます
		GameOverDispatcer.Broadcast();
	}
}

// エネミー判断
bool APlayerCharacter::IsEnemy(AActor* OtherActor)
{
	if (OtherActor)
	{
		// 衝突したアクターがARazorKillerクラスのインスタンスであるかをチェックする
		if (OtherActor->IsA(ARazorKiller::StaticClass()))
		{
			//デバッグ用ログ
			//UE_LOG(LogTemp, Warning, TEXT("Correct Now"));
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
//---------------------------------------------------

// 2024/05/09 追加
// アイテムを
void APlayerCharacter::AddItemToInventory(AItemBase* Item)
{
	if (Item)
	{
		Inventory.Add(Item);
	}
}

void APlayerCharacter::PrintInventory()
{
	UE_LOG(LogTemp, Display, TEXT("Player Inventory:"));

	for (AItemBase* Item : Inventory)
	{
		UE_LOG(LogTemp, Display, TEXT("- Item ID: %d"), Item->ItemData.ID);
	}
}
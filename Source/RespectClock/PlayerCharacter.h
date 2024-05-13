//-------------------------------------------------------------------------------------
// ファイル名：PlayerCharacter.h
// 処理内容	：Playerの挙動を制御をするキャラクタークラス
// 作成者	：日向野大聖
// 作成日	：04/08
// 更新日	：04/08		プレイヤーの移動処理追加
//			　
//-------------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "E_PlayerState.h"
#include "InteractiveInput.h"
#include "PlayerCharacter.generated.h"

//前方宣言
class UInputMappingContext;
class UInputAction;
class UMoveInput;
class AItemBase;	// 05/10追加
class UBoxComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT(BlueprintType)
struct F_PlayerParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "CharaState")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float RunSpeed_100;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float RunSpeed_75;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float RunSpeed_50;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float RunSpeed_25;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float Stamina;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float StaminaDecreasedValue;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float StaminaRecoverdValue_Idle;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float StaminaRecoverdValue_Walk;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float StaminaRecoverdValue_Sit;
};

UCLASS(config = Game)
class RESPECTCLOCK_API APlayerCharacter : public ACharacter
{

	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	/** Sit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SitAction;

	/** Action Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractiveAction;

	/** Dog Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DogAction;

	/** Dog Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ItemAction;

public:
	APlayerCharacter();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void StopMove();

	void Run();
	void StopRun();

	void Sit();

	void Interactive();

	void Dog(const FInputActionValue& Value);

	void ItemSlect(const FInputActionValue& Value);

	void RecoverStamina();

public:
	UFUNCTION(BlueprintCallable, category = "CharacterAction")
	void Hide();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, Category = "CharaState")
	float InputValueMin;
	UPROPERTY()
	UMoveInput* MoveInput;
	UPROPERTY()
	UInteractiveInput* InteractiveInput;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMoveable;
	UPROPERTY(EditAnywhere, Category = "CharaState")
	F_PlayerParam PlayerParam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_PlayerState PlayerStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_PlayerStaminaState PlayerStaminaPercentage;
	UPROPERTY()
	FVector HidePos;
	UPROPERTY()
	FRotator HideRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHiding;

	//-----22cu0127 丹代天 2024/05/07 追記：衝突判定・ゲームオーバー時に呼び出すイベントディスパッチャー-----
// イベントディスパッチャー
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverDispatcer);
public:
	UPROPERTY(BlueprintAssignable, Category = "Custom Events")
	FGameOverDispatcer GameOverDispatcer;

private:
	// 衝突判定
	UFUNCTION()
	void OnHitActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// エネミー判断
	bool IsEnemy(AActor* OtherActor);
	//衝突判定を実行するかどうか判断するフラグ
public:
	UPROPERTY(EditAnywhere, Category = "CollisionCheck")
	bool m_bCollisionCheck = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pBoxComp;

	//------------------------------------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AItemBase*> Inventory;	// インベントリ

	void AddItemToInventory(AItemBase* Item);	// アイテムをインベントリに追加
	TArray<AItemBase*> GetInventory()const { return Inventory; }	// インベントリの内容を返す

	UFUNCTION(BlueprintCallable)
	void PrintInventory();	// インベントリの内容をログで出力

	// カスコード(田中)
	UFUNCTION(BlueprintImplementableEvent, Category = "MyF")
	void BPKillerSummon();
};

/*
* FileName	: AIC_RazorKiller.h
* Created	: Misora Tanaka
* Summary	: Program to control the Razor Killer AI
* Update	: 24/04/08 Create
* 
*/

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_RazorKiller.generated.h"	// ここより下にヘッダーを書かない

// 前方宣言
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class APlayerCharacter;
class ATriggerBox;

UCLASS()
class RESPECTCLOCK_API AAIC_RazorKiller : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_RazorKiller(const class FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	void SetPlayerKey(TObjectPtr<APawn> const player) const;
	void SetChaseKey() const;
	void DeletePlayerKey() const;

	TObjectPtr<APlayerCharacter> GetPlayerKey() const;

public:
	// 
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PlayerKeyName;

protected:
	// AIControllerのPawn所持
	virtual void OnPossess(class APawn* InPawn) override;

	// AIControllerのPawn所持解除
	virtual void OnUnPossess() override;
protected:

	UPROPERTY(EditDefaultsOnly, Category= "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

public:
	void SetMoveArea() const; 
	void SetWarpPoint() const;
	void SetStartPoint() const;
	// マジでゴミだからさっさと直せよ
	UFUNCTION(BlueprintCallable, Category= "Trash")
	void Summon() const;
private:
	TArray<TObjectPtr<ATriggerBox>> MoveAreas;
};

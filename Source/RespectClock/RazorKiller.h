// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RazorKiller.generated.h"

// 前方宣言
class UPawnSensingComponent;
class AAIC_RazorKiller;

UCLASS()
class RESPECTCLOCK_API ARazorKiller : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARazorKiller();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);
private:
	UFUNCTION()
	void LostSeePlayer() const;
	UFUNCTION()
	void DebugPringString(FString Log, FColor Color,float PrintTime) const;
	UFUNCTION()
	void DebugLog(FString Log) const;
	
public:
	UPROPERTY(EditAnywhere,Category = "AI")
	float SeverityPoint;
	UFUNCTION(BlueprintCallable, Category = "AI")
	float GetSeverityPoint(){ return SeverityPoint; }
private:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> SensingComp;
	UPROPERTY()
	TObjectPtr<AAIC_RazorKiller> AIRazorKiller;

	UPROPERTY(EditAnywhere, Category = "AI")
	float VisionAngle;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	FTimerHandle ChaseHandle;
	UPROPERTY(EditAnywhere, Category = "AI")
	float ChaseTime;
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowLog;
	UPROPERTY(EditAnywhere, Category = "AI")
	bool IsChaseTime;
};
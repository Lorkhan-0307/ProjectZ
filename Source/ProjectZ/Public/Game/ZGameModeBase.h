// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZGameModeBase.generated.h"

class AZCharacterBase;
class UCharacterClassInfo;

UENUM(BlueprintType)
enum class ETurn : uint8
{
	// NonCombat
	ET_NonCombat UMETA(DisplayName = "NonCombat"),
	// Combat
	ET_MoveTurn UMETA(DisplayName = "MoveTurn"),
	ET_PlayerTurn UMETA(DisplayName = "Player Turn"),
	ET_EnemyTurn UMETA(DisplayName = "Enemy Turn")};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnChangedDelegate, ETurn, CurrentTurn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatActorChangedDelegate, AActor*, Actor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatStartDelegate, TArray<AActor*>&, CombatActor);

UCLASS()
class PROJECTZ_API AZGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AActor*> CombatActor;

	void CombatStart(int roomNo);
	UFUNCTION(BlueprintCallable)
	void CombatEnd();
	void TurnEnd();
	void CharacterDie(AActor* DieActor);

	UFUNCTION(BlueprintCallable)
	void SetTurn(ETurn Turn);

	FORCEINLINE void SetLastTurn(ETurn Turn) { LastTurn = Turn; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETurn GetCurrentTurn() const { return CurrentTurn; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETurn GetLastTurn() const { return LastTurn; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FTurnChangedDelegate TurnChangedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FCombatActorChangedDelegate CombatActorChangedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FCombatStartDelegate CombatStartDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	FVector2D ScreenSize = FVector2D(1920.f, 1080.f);

	FORCEINLINE int32 GetTurnPlayerIndex() const { return TurnPlayerIndex; }

	void GetCombatActor(TArray<AActor*>& CombatActors);

	UPROPERTY()
	AActor* TurnActor;

	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	float TurnEndTime = 2.f;

private:
	ETurn CurrentTurn = ETurn::ET_NonCombat;
	ETurn LastTurn;

	UPROPERTY()
	TArray<AZCharacterBase*> CombatCharacter;

	int32 TurnPlayerIndex;

	FTimerHandle TurnEndTimer;

	void SortCombatActor();
	void TurnEndWithTime();

	UFUNCTION()
	void NextTurn();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZGameModeBase.generated.h"

class UCharacterClassInfo;

UENUM(BlueprintType)
enum class ETurn : uint8
{
	// NonCombat
	ET_NonCombat UMETA(DisplayName = "NonCombat"),
	// Combat
	ET_PlayerTurn UMETA(DisplayName = "Player Turn"),
	ET_EnemyTurn UMETA(DisplayName = "Enemy Turn")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnChangedDelegate, ETurn, CurrentTurn);

UCLASS()
class PROJECTZ_API AZGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetTurn(ETurn Turn);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETurn GetCurrentTurn() const { return CurrentTurn; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FTurnChangedDelegate TurnChangedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

private:
	ETurn CurrentTurn = ETurn::ET_NonCombat;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/ZGameModeBase.h"
#include "Character/ZCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "ZEnemy.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UBehaviorTree;
class AZAIController;

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZEnemy : public AZCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AZEnemy();

	virtual void PossessedBy(AController* NewController) override;

	// Combat Interface
	virtual int32 GetPlayerLevel() override;

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	bool bHitReacting;

	bool bIsMyTurn = false;

	bool bTargetSet = false;

	FORCEINLINE AZAIController* GetAIController() const { return ZAIController; }

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AZAIController> ZAIController;

	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIPerceptionStimuliSourceComponent;

private:
	UFUNCTION()
	void TurnChanged(ETurn Turn);
};

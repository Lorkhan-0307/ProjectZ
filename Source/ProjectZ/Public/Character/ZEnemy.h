// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "ZEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZEnemy : public AZCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AZEnemy();

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

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};

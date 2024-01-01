// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ZGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FGameplayTag StartupInputTag;

	// If the ability is free of damage, do not use it
	// Can set curve table (CSV, JSON, CurveTable in editor)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
	FScalableFloat Damage;
};

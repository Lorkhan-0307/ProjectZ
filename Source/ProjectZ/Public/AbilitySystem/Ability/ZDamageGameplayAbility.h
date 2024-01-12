// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ZGameplayAbility.h"
#include "ZDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZDamageGameplayAbility : public UZGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TMap<FGameplayTag,FScalableFloat> DamageType;
};

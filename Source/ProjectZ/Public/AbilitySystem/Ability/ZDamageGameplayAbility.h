// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZAbilityType.h"
#include "AbilitySystem/Ability/ZGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "ZDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZDamageGameplayAbility : public UZGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = Debuff)
	TArray<FGameplayTag> DebuffTypes;

	UPROPERTY(EditDefaultsOnly, Category = Debuff)
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = Debuff)
	float DebuffDamage = 5.f;

	// By Turn
	UPROPERTY(EditDefaultsOnly, Category = Debuff)
	int32 DebuffDuration = 1;

	UPROPERTY(EditDefaultsOnly, Category = Debuff)
	int32 DebuffStack = 1;


	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};

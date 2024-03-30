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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
	FGameplayTag CardType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Debuff)
	FGameplayTag DebuffType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Debuff)
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Debuff)
	float DebuffDamage = 5.f;

	// By Turn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Debuff)
	int32 DebuffDuration = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Debuff)
	int32 DebuffStack = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	UAnimMontage* AbilityMontage;


	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CardEffect();
};

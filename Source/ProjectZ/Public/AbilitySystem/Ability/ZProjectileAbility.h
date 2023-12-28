// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ZGameplayAbility.h"
#include "ZProjectileAbility.generated.h"

class AZProjectile;
class UGameplayEffect;

UCLASS()
class PROJECTZ_API UZProjectileAbility : public UZGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = Projectile)
	void SpawnProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AZProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};

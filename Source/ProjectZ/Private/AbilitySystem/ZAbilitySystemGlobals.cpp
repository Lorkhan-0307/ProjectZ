// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZAbilitySystemGlobals.h"

#include "ZAbilityType.h"

FGameplayEffectContext* UZAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FZGameplayEffectContext();
}

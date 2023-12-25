// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZAbilitySystemComponent.h"

#include "ZGameplayTag.h"

void UZAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UZAbilitySystemComponent::EffectApplied);

	// To get Gameplay Tags
	//const FZGameplayTag& GameplayTag = FZGameplayTag::Get();
}

void UZAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ZDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"

void UZDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(TargetActor);

	UZAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
}

FDamageEffectParams UZDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage;
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffType = DebuffType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffStack = DebuffStack;
	Params.BuffType = BuffType;
	Params.BuffAttribute = BuffAttribute;
	Params.BuffMagnitude = BuffMagnitude;
	Params.BuffDuration = BuffDuration;
	return Params;
}

void UZDamageGameplayAbility::ApplyBuff()
{
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(""));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	Effect->InheritableOwnedTagsContainer.AddTag(BuffType);

	Effect->StackingType = EGameplayEffectStackingType::AggregateByTarget;
	Effect->StackLimitCount = 20;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(BuffMagnitude);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	//ModifierInfo.Attribute = AttributeSet;

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(Effect, 1, EffectContext);
	ICombatInterface::Execute_AddBuff(GetAvatarActorFromActorInfo(), BuffType, BuffDuration);
}

FTaggedMontage UZDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() == 0) return FTaggedMontage();

	const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
	return TaggedMontages[Selection];
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalcDamage.h"

#include "AbilitySystemComponent.h"
#include "ZAbilityType.h"
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct ZDamageStatic
{
	// Declare from AttributeSet
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DodgeChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;

	ZDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, DodgeChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UZAttributeSet, PhysicalResistance, Target, false);

		const FZGameplayTag& Tags = FZGameplayTag::Get();
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_DodgeChance, DodgeChanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagsToCaptureDef.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const ZDamageStatic& DamageStatic()
{
	static ZDamageStatic DStatic;
	return DStatic;
}

UExecCalcDamage::UExecCalcDamage()
{
	RelevantAttributesToCapture.Add(DamageStatic().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatic().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatic().DodgeChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatic().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().PhysicalResistanceDef);
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTag;
	EvaluationParameters.TargetTags = TargetTag;

	// Debuff
	const FZGameplayTag& GameplayTag = FZGameplayTag::Get();
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	for (FGameplayTag DebuffType : GameplayTag.Debuffs)
	{
		const bool bDebuffIsSet = Spec.GetSetByCallerMagnitude(DebuffType, false, -1.f) > -0.5f; // .5 padding for floating point imprecision
		if (bDebuffIsSet)
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTag.Debuff_Chance, false, -1.f);
			const bool bDebuff = FMath::RandRange(1, 100) < SourceDebuffChance;
			if (bDebuff)
			{
				UZAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UZAbilitySystemLibrary::SetDebuffType(ContextHandle, DebuffType);
				//UZAbilitySystemLibrary::SetDamageType(ContextHandle,DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTag.Debuff_Damage, false, -1.f);
				const int32 DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTag.Debuff_Duration, false, -1.f);
				const int32 DebuffStack = Spec.GetSetByCallerMagnitude(GameplayTag.Debuff_Stack, false, -1.f);

				UZAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UZAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UZAbilitySystemLibrary::SetDebuffStack(ContextHandle, DebuffStack);
			}
		}
	}

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (const auto& Pair : GameplayTag.DamageTypeToResistance)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(ZDamageStatic().TagsToCaptureDef.Contains(ResistanceTag), TEXT("TagsToCaptureDef doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());

		const FGameplayEffectAttributeCaptureDefinition CaptureDef = ZDamageStatic().TagsToCaptureDef[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;

		Damage += DamageTypeValue;
	}

	// Example of Damage calculation

	// Capture DodgeChance On Target, and determine if there was a successful Dodge
	// If Dodge, 0 damage

	float TargetDodgeChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DodgeChanceDef, EvaluationParameters, TargetDodgeChance);
	TargetDodgeChance = FMath::Max<float>(TargetDodgeChance, 0.f);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetDodgeChance;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UZAbilitySystemLibrary::SetIsDodged(EffectContextHandle, bBlocked);

	Damage = bBlocked ? 0.f : Damage;

	// ArmorPenetration ignores a percentage of the Target's Armor

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(TargetArmor, 0.f);

	// ArmorPenetration ignores percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration) / 100.f;
	// Armor ignores percentage of incoming damage
	Damage *= (100.f - EffectiveArmor) / 100.f;

	// Critical
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	// Critical Hit Resistance reduces Critical Hit chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UZAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	Damage = bCriticalHit ? SourceCriticalHitDamage * Damage : Damage;

	// Apply Damage
	const FGameplayModifierEvaluatedData EvaluatedData(UZAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

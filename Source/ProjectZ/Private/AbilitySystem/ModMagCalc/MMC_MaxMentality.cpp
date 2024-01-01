// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMentality.h"

#include "AbilitySystem/ZAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMentality::UMMC_MaxMentality()
{
	AntiSociabilityDef.AttributeToCapture = UZAttributeSet::GetAntiSociabilityAttribute();
	AntiSociabilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AntiSociabilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AntiSociabilityDef);
}

float UMMC_MaxMentality::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float AntiSociability = 0.f;
	GetCapturedAttributeMagnitude(AntiSociabilityDef, Spec, EvaluateParameters, AntiSociability);
	AntiSociability = FMath::Max<float>(AntiSociability, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	// Base Value + 10 * AntiSociability Level
	// If game need player level, change value 0 to some value
	return 100.f + 10.f * AntiSociability + 0.f * PlayerLevel;
}
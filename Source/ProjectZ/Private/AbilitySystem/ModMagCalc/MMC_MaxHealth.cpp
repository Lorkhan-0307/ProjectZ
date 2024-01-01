// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/ZAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	SociabilityDef.AttributeToCapture = UZAttributeSet::GetSociabilityAttribute();
	SociabilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	SociabilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(SociabilityDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Sociability = 0.f;
	GetCapturedAttributeMagnitude(SociabilityDef, Spec, EvaluateParameters, Sociability);
	Sociability = FMath::Max<float>(Sociability, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	// Base Value + 10 * Sociability Level
	// If game need player level, change value 0 to some value
	return 100.f + 10.f * Sociability + 0.f * PlayerLevel;
}

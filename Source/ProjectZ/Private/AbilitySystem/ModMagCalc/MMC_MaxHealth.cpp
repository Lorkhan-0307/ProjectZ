// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/ZAttributeSet.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	SociabilityDef.AttributeToCapture = UZAttributeSet::GetSociabilityAttribute();
	SociabilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	SociabilityDef.bSnapshot=false;
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return 0;
}

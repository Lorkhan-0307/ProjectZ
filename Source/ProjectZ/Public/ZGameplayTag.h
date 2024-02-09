// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// Singleton containing native Gameplay Tags for using Gameplay Tags in C++
struct FZGameplayTag
{
public:
	static const FZGameplayTag& Get() {return GameplayTag;}
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attributes_Primary_Sociability;
	FGameplayTag Attributes_Primary_AntiSociability;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Engineering;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMentality;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_DodgeChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Physical;

	// For Test
	FGameplayTag InputTag_T;

	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_LMB;
	//FGameplayTag Event_Montage_ThrowingStone;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Physical;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToResistance;

	FGameplayTag Effect_HitReact;

	FGameplayTag Card_SKill_ThrowStone;
private:
	static FZGameplayTag GameplayTag;
};

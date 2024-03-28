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

	// For Test
	FGameplayTag InputTag_T;

	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_LMB;
	
	//FGameplayTag Event_Montage_ThrowingStone;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Physical;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Debuff_Bleed;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Slow;
	FGameplayTag Debuff_KnockBack;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Stack;

	TArray<FGameplayTag> Debuffs;

	FGameplayTag Ability_Attack;

	FGameplayTag Montage_Attack_Weapon;
	FGameplayTag Montage_Attack_RightHand;
	FGameplayTag Montage_Attack_LeftHand;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypeToResistance;

	FGameplayTag Effect_HitReact;
	
	FGameplayTag Card_Weapon_OneHand;
	FGameplayTag Card_Weapon_TwoHand;
	FGameplayTag Card_SKill_ThrowStone;
	FGameplayTag Card_Skill_Swing;
	FGameplayTag Card_Skill_Blocking;
	FGameplayTag Card_Skill_Smash;
	FGameplayTag Card_Skill_Frenzy;
	FGameplayTag Card_Skill_Gather;

private:
	static FZGameplayTag GameplayTag;
};

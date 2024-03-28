// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameplayTag.h"
#include "GameplayTagsManager.h"
#include "NativeGameplayTags.h"

FZGameplayTag FZGameplayTag::GameplayTag;

void FZGameplayTag::InitializeNativeGameplayTags()
{
	GameplayTag.Attributes_Primary_Sociability = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Sociability"));
	GameplayTag.Attributes_Primary_AntiSociability = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.AntiSociability"));
	GameplayTag.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"));
	GameplayTag.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"));
	GameplayTag.Attributes_Primary_Engineering = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Engineering"));

	GameplayTag.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"));
	GameplayTag.Attributes_Secondary_MaxMentality = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMentality"));
	GameplayTag.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"));
	GameplayTag.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"));
	GameplayTag.Attributes_Secondary_DodgeChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.DodgeChance"));
	GameplayTag.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"));
	GameplayTag.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"));
	GameplayTag.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"));

	GameplayTag.Attributes_Card_WeaponAtk = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Card.WeaponAtk"));
	GameplayTag.Attributes_Card_Defence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Card.Defence"));

	// For Test
	GameplayTag.InputTag_T = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.T"));

	GameplayTag.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"));
	GameplayTag.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"));

	GameplayTag.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"));

	// Damage Type
	GameplayTag.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"));
	GameplayTag.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"));
	GameplayTag.DamageTypeToResistance.Add(GameplayTag.Damage_Fire);

	// Resistance
	GameplayTag.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"));
	GameplayTag.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"));

	// Map of DamageType to Resistance
	GameplayTag.DamageTypeToResistance.Add(GameplayTag.Damage_Fire, GameplayTag.Attributes_Resistance_Fire);
	GameplayTag.DamageTypeToResistance.Add(GameplayTag.Damage_Physical, GameplayTag.Attributes_Resistance_Physical);

	// Debuff
	GameplayTag.Debuff_Bleed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Bleed"));
	GameplayTag.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"));
	GameplayTag.Debuff_Slow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Slow"));
	GameplayTag.Debuff_KnockBack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.KnockBack"));

	GameplayTag.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"));
	GameplayTag.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"));
	GameplayTag.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"));
	GameplayTag.Debuff_Stack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stack"));

	// Array of Debuffs
	GameplayTag.Debuffs.Add(GameplayTag.Debuff_Bleed);
	GameplayTag.Debuffs.Add(GameplayTag.Debuff_Slow);
	GameplayTag.Debuffs.Add(GameplayTag.Debuff_Stun);

	// Effect
	GameplayTag.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"), FString("Tag granted when Hit React"));

	// Ability
	GameplayTag.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"));

	// Card
	GameplayTag.Card_Level = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Level"));
	
	GameplayTag.Card_Weapon_OneHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Weapon.OneHand"));
	GameplayTag.Card_Weapon_TwoHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Weapon.TwoHand"));
	
	GameplayTag.Card_SKill_ThrowStone = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Skill.ThrowStone"));
	GameplayTag.Card_Skill_Swing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Skill.Swing"));
	GameplayTag.Card_Skill_Blocking = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Skill.Blocking"));
	GameplayTag.Card_Skill_Smash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Skill.Smash"));
	GameplayTag.Card_Skill_Frenzy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Skill.Frenzy"));
	GameplayTag.Card_Skill_Gather = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Card.Skill.Gather"));
	

	// Montage
	GameplayTag.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon"));
	GameplayTag.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.RightHand"));
	GameplayTag.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.LeftHand"));
	
}

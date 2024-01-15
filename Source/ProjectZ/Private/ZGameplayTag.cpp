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

	// For Test
	GameplayTag.InputTag_T = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.T"));

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
	// Effect

	GameplayTag.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"), FString("Tag granted when Hit React"));
}

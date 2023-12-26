// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameplayTag.h"
#include "GameplayTagsManager.h"

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

	// For Test
	GameplayTag.InputTag_T = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.T"));
}

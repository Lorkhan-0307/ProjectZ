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

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMentality;
	
	FGameplayTag Attributes_Primary_Sociability;
	FGameplayTag Attributes_Primary_AntiSociability;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Engineering;

	// For Test
	FGameplayTag InputTag_T;
	//FGameplayTag Event_Montage_ThrowingStone;

	FGameplayTag Damage;

	FGameplayTag Effect_HitReact;
private:
	static FZGameplayTag GameplayTag;
};

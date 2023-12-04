// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZPlayerState.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAttributeSet.h"

AZPlayerState::AZPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UZAbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UZAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AZPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
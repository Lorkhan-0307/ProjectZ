// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WidgetController/NonCombatOverlayWidgetController.h"
#include "AbilitySystem/ZAttributeSet.h"

void UNonCombatOverlayWidgetController::BroadcastInitialValues()
{
	const UZAttributeSet* AuraAttributeSet = CastChecked<UZAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}

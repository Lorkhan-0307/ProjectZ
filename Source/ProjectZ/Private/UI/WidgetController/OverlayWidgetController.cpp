// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/ZAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UZAttributeSet* ZAttributeSet = CastChecked<UZAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(ZAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ZAttributeSet->GetMaxHealth());
}

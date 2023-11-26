// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WidgetController/ZWidgetController.h"

void UZWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttruAttributeSet;
}

void UZWidgetController::BroadcastInitialValues()
{
}

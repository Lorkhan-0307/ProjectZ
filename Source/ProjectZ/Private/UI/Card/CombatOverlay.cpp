// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Card/CombatOverlay.h"
#include "UI/Card/CardHandWidget.h"
#include "Character/ZNonCombatCharacter.h"
#include "Character/CardComponent.h"
#include "Components/ProgressBar.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	CardHandWidget->SetCardComponent(CardComponent);
}

void UCombatOverlay::WidgetControllerSet()
{
	Super::WidgetControllerSet();
}

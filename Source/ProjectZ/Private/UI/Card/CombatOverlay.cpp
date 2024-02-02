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

	UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);
	OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UCombatOverlay::OnHealthChanged);
	OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UCombatOverlay::OnMaxHealthChanged);
	OverlayWidgetController->OnMentalityChanged.AddDynamic(this, &UCombatOverlay::OnMentalityChanged);
	OverlayWidgetController->OnMaxMentalityChanged.AddDynamic(this, &UCombatOverlay::OnMaxMentalityChanged);
}

void UCombatOverlay::OnHealthChanged(float NewValue)
{
	Health = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
}

void UCombatOverlay::OnMaxHealthChanged(float NewValue)
{
	MaxHealth = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
}

void UCombatOverlay::OnMentalityChanged(float NewValue)
{
	Mentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
}

void UCombatOverlay::OnMaxMentalityChanged(float NewValue)
{
	MaxMentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
}
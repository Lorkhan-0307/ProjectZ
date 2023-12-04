// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Card/CombatOverlay.h"
#include "UI/Card/CardHandWidget.h"
#include "Character/ZCharacter.h"
#include "Character/CardComponent.h"
#include "Components/ProgressBar.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	CardHandWidget->SetCardComponent(CardComponent);

	//CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
	// This Line has been moved to the Initoverlay function in the ZHUDbase code.
}

/*
void UCombatOverlay::OnHealthChanged(float NewValue)
{
	Health = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
	UE_LOG(LogTemp,Warning,TEXT("Health = %f %f %f"),Health,MaxHealth,Health/MaxHealth);
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
*/

void UCombatOverlay::WidgetControllerSet()
{
	Super::WidgetControllerSet();
}

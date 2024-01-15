// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/NonCombat/NonCombatOverlay.h"

#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Ui/Card/CardWidget.h"
#include "Player/ZNonCombatPlayerController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/ProgressBar.h"


void UNonCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();
}

void UNonCombatOverlay::SetCardComponent(UCardComponent* CC)
{
	Super::SetCardComponent(CC);
	if (CardComponent)
	{
		CardComponent->UpdateLeftHandCardDelegate.AddDynamic(this, &UNonCombatOverlay::UpdateLeftHandCard);
		CardComponent->UpdateRightHandCardDelegate.AddDynamic(this, &UNonCombatOverlay::UpdateRightHandCard);
	}
}

void UNonCombatOverlay::UpdateLeftHandCard(FCard LeftCard)
{
	if (LeftCard.IsValid == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	LeftHandCardWidget->InitCardStatus(LeftCard);
}

void UNonCombatOverlay::UpdateRightHandCard(FCard RightCard)
{
	if (RightCard.IsValid == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	RightHandCardWidget->InitCardStatus(RightCard);
}

void UNonCombatOverlay::OnHealthChanged(float NewValue)
{
	Health = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
}

void UNonCombatOverlay::OnMaxHealthChanged(float NewValue)
{
	MaxHealth = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
}

void UNonCombatOverlay::OnMentalityChanged(float NewValue)
{
	Mentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
}

void UNonCombatOverlay::OnMaxMentalityChanged(float NewValue)
{
	MaxMentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
}

void UNonCombatOverlay::WidgetControllerSet()
{
	UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);
	OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UNonCombatOverlay::OnHealthChanged);
	OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UNonCombatOverlay::OnMaxHealthChanged);
	OverlayWidgetController->OnMentalityChanged.AddDynamic(this, &UNonCombatOverlay::OnMentalityChanged);
	OverlayWidgetController->OnMaxMentalityChanged.AddDynamic(this, &UNonCombatOverlay::OnMaxMentalityChanged);
}

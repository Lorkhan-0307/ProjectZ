// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/NonCombat/NonCombatOverlay.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Ui/Card/CardWidget.h"
#include "Player/ZNonCombatPlayerController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/ProgressBar.h"


void UNonCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	HealthCheckingBar->SetVisibility(ESlateVisibility::Visible);
	MentalityCheckingBar->SetVisibility(ESlateVisibility::Visible);
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

void UNonCombatOverlay::WidgetControllerSet()
{
	Super::WidgetControllerSet();
	UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);
	OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UNonCombatOverlay::OnHealthChanged);
	OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UNonCombatOverlay::OnMaxHealthChanged);
	OverlayWidgetController->OnMentalityChanged.AddDynamic(this, &UNonCombatOverlay::OnMentalityChanged);
	OverlayWidgetController->OnMaxMentalityChanged.AddDynamic(this, &UNonCombatOverlay::OnMaxMentalityChanged);
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

float UNonCombatOverlay::GetHealthCheckingBarPos()
{
	const UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HealthBar);
	return CanvasPanelSlot->GetSize().X * HealthBar->GetPercent() + CanvasPanelSlot->GetPosition().X;
}

float UNonCombatOverlay::GetMentalityCheckingBarPos()
{
	const UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(MentalityBar);
	return CanvasPanelSlot->GetSize().X * MentalityBar->GetPercent() + CanvasPanelSlot->GetPosition().X;
}

void UNonCombatOverlay::SetHealthCheckingBarPos()
{
	HealthCheckingBar->SetVisibility(ESlateVisibility::Visible);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HealthCheckingBar);
	CanvasPanelSlot->SetPosition(FVector2D(GetHealthCheckingBarPos(), 0.f));
}

void UNonCombatOverlay::SetMentalityCheckingBarPos()
{
	MentalityCheckingBar->SetVisibility(ESlateVisibility::Visible);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(MentalityCheckingBar);
	CanvasPanelSlot->SetPosition(FVector2D(GetMentalityCheckingBarPos(), 0.f));
}

void UNonCombatOverlay::OnHealthChanged(float NewValue)
{
	Health = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
	SetHealthCheckingBarPos();
}

void UNonCombatOverlay::OnMaxHealthChanged(float NewValue)
{
	MaxHealth = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
	SetHealthCheckingBarPos();
}

void UNonCombatOverlay::OnMentalityChanged(float NewValue)
{
	Mentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
	SetMentalityCheckingBarPos();
}

void UNonCombatOverlay::OnMaxMentalityChanged(float NewValue)
{
	MaxMentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
	SetMentalityCheckingBarPos();
}

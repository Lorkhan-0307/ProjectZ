// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZUserWidget.h"

#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/ProgressBar.h"

void UZUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UZUserWidget::SetCardComponent(UCardComponent* CC)
{
	CardComponent = CC;
}

void UZUserWidget::OnHealthChanged(float NewValue)
{
	Health = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
}

void UZUserWidget::OnMaxHealthChanged(float NewValue)
{
	MaxHealth = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
}

void UZUserWidget::OnMentalityChanged(float NewValue)
{
	Mentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
}

void UZUserWidget::OnMaxMentalityChanged(float NewValue)
{
	MaxMentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
}

void UZUserWidget::WidgetControllerSet()
{
	UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);
	OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UZUserWidget::OnHealthChanged);
	OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UZUserWidget::OnMaxHealthChanged);
	OverlayWidgetController->OnMentalityChanged.AddDynamic(this, &UZUserWidget::OnMentalityChanged);
	OverlayWidgetController->OnMaxMentalityChanged.AddDynamic(this, &UZUserWidget::OnMaxMentalityChanged);
}


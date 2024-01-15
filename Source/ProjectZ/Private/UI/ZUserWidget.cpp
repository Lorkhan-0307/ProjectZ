// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZUserWidget.h"

#include "UI/WidgetController/OverlayWidgetController.h"

void UZUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UZUserWidget::SetCardComponent(UCardComponent* CC)
{
	CardComponent = CC;
}

void UZUserWidget::WidgetControllerSet()
{
}




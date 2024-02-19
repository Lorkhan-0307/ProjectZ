// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombat/DialogWidget.h"

#include "Components/Button.h"

void UDialogWidget::WidgetControllerSet()
{
	Super::WidgetControllerSet();

	Select1Button->OnPressed.AddDynamic(this, &UDialogWidget::Select1ButtonPressed);
	Select2Button->OnPressed.AddDynamic(this, &UDialogWidget::Select2ButtonPressed);
	Select2Button->OnPressed.AddDynamic(this, &UDialogWidget::Select2ButtonPressed);
}

void UDialogWidget::Select1ButtonPressed()
{
}

void UDialogWidget::Select2ButtonPressed()
{
}

void UDialogWidget::Select3ButtonPressed()
{
}

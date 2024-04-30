// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombat/InventoryWidget.h"

#include "Character/CardComponent.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/NonCombat/BackpackWidget.h"
#include "UI/NonCombat/CraftWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void UInventoryWidget::WidgetControllerSet()
{
	Super::WidgetControllerSet();
	UInventoryWidgetController* InventoryWidgetController = Cast<UInventoryWidgetController>(WidgetController);
	CardComponent->UpdateCardInventoryDelegate.AddDynamic(this, &UInventoryWidget::UpdateCardInventory);

	BackpackWidget = CreateWidget<UBackpackWidget>(GetWorld(), BackpackWidgetClass);
	BackpackWidget->CardComponent = CardComponent;
	InventorySwitcher->AddChild(BackpackWidget);
	CraftWidget = CreateWidget<UCraftWidget>(GetWorld(), CraftWidgetClass);
	InventorySwitcher->AddChild(CraftWidget);
}

void UInventoryWidget::DisplayCardWidget(FCard DisplayCard)
{
	BackpackWidget->DisplayCardWidget(DisplayCard);
}

void UInventoryWidget::UpdateCardInventory()
{
	BackpackWidget->MakeBackpack();
}

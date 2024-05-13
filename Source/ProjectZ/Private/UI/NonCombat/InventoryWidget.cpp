// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombat/InventoryWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/CardComponent.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WidgetSwitcherSlot.h"
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
	CraftWidget->CardComponent = CardComponent;
	InventorySwitcher->AddChild(CraftWidget);

	OnVisibilityChanged.AddDynamic(this, &UInventoryWidget::VisibilityChanged);
}

void UInventoryWidget::DisplayCardWidget(FCard DisplayCard)
{
	BackpackWidget->DisplayCardWidget(DisplayCard);
}

void UInventoryWidget::UpdateCardInventory()
{
	BackpackWidget->MakeBackpack();
	CraftWidget->MakeBackpack();
	CraftWidget->HideFirstCard();
	CraftWidget->HideSecondCard();
}

void UInventoryWidget::VisibilityChanged(ESlateVisibility InVisibility)
{
	//BackpackWidget->WidgetSize.X = FMath::Max(BackpackWidget->GetCachedGeometry().GetLocalSize().X, BackpackWidget->WidgetSize.X);
	//CraftWidget->BackpackWidget->WidgetSize.X = FMath::Max(CraftWidget->BackpackWidget->GetCachedGeometry().GetLocalSize().X, CraftWidget->BackpackWidget->WidgetSize.X);
	UpdateCardInventory();

	if (InVisibility == ESlateVisibility::Hidden)
	{
		BackpackWidget->DestroyDisplayCardWidget();
	}
}

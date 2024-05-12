// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombat/BackpackWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Character/CardComponent.h"
#include "Character/ZPlayerCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "UI/Card/CardWidget.h"
#include "UI/NonCombat/CardDisplayWidget.h"

void UBackpackWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBackpackWidget::MakeBackpack()
{
	if (CardComponent == nullptr) return;

	TArray<FCard> CardInventory;
	CardComponent->SortInventory();
	CardComponent->GetCardInventory(CardInventory);
	if (CardInventory.Num() == 0) return;

	CardRowAmount = WidgetSize.X / CardSize.X;

	if (CardRowAmount <= 0) return;

	CardScrollBox->ClearChildren();
	int CardNum = 0;
	for (int i = 0; i < CardInventory.Num() / CardRowAmount + 1; i++)
	{
		UHorizontalBox* HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
		for (int j = 0; j < CardRowAmount; j++)
		{
			UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetWorld(), CardWidgetClass);
			FCard Card = CardInventory[CardNum];
			Card.IsValid = false;
			CardWidget->InitCardStatus(Card, false);
			//CardWidget->ClickCardWidgetDelegate->AddDynamic(this,&UBackpackWidget::DisplayCardWidget);
			HorizontalBox->AddChild(CardWidget);
			CardNum++;
			if (CardNum == CardInventory.Num()) break;
		}
		CardScrollBox->AddChild(HorizontalBox);
		if (CardNum == CardInventory.Num()) break;
	}
}

void UBackpackWidget::DisplayCardWidget(FCard ClickCard)
{
	CardDisplayWidget = CreateWidget<UCardDisplayWidget>(GetWorld(), CardDisplayWidgetClass);
	CardDisplayWidget->AddToViewport();
	CardDisplayWidget->SetDisplayCard(ClickCard);
}

void UBackpackWidget::DestroyDisplayCardWidget()
{
	if (CardDisplayWidget) CardDisplayWidget->DestroyDisplayCard();
}

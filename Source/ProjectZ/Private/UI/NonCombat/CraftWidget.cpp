// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombat/CraftWidget.h"
#include "Character/CardComponent.h"
#include "Components/Button.h"
#include "UI/Card/CardWidget.h"
#include "UI/NonCombat/BackpackWidget.h"

void UCraftWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FirstCardWidget->SetVisibility(ESlateVisibility::Hidden);
	SecondCardWidget->SetVisibility(ESlateVisibility::Hidden);
	ResultCardWidget->SetVisibility(ESlateVisibility::Hidden);

	CraftButton->OnClicked.AddDynamic(this, &UCraftWidget::Craft);
	FirstCardWidgetButton->OnClicked.AddDynamic(this, &UCraftWidget::HideFirstCard);
	SecondCardWidgetButton->OnClicked.AddDynamic(this, &UCraftWidget::HideSecondCard);
}

void UCraftWidget::MakeBackpack()
{
	BackpackWidget->MakeBackpack();
	BackpackWidget->CardComponent = BackpackWidget->CardComponent == nullptr ? CardComponent : BackpackWidget->CardComponent;
}

void UCraftWidget::SetFirstCardWidget(FCard& CardStatus, UCraftCardWidget* CraftCardWidget)
{
	FirstCardWidget->InitCardStatus(CardStatus);
	FirstCardWidget->SetVisibility(ESlateVisibility::Visible);
	FirstCard = CardStatus;

	CancleFirstCard();
	LastFirstCardWidget = CraftCardWidget;

	ShowCraftResult();
}

void UCraftWidget::SetSecondCardWidget(FCard& CardStatus, UCraftCardWidget* CraftCardWidget)
{
	SecondCardWidget->InitCardStatus(CardStatus);
	SecondCardWidget->SetVisibility(ESlateVisibility::Visible);
	SecondCard = CardStatus;

	CancleSecondCard();
	LastSecondCardWidget = CraftCardWidget;

	ShowCraftResult();
}

void UCraftWidget::ShowCraftResult()
{
	if (FirstCardWidget->GetVisibility() == ESlateVisibility::Visible && SecondCardWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		bool bIsValid;
		const FCard ResultCard = CardComponent->GetCardCraftResult(FirstCardWidget->GetCardStat(), SecondCardWidget->GetCardStat(), bIsValid);
		if (bIsValid)
		{
			ResultCardWidget->SetVisibility(ESlateVisibility::Visible);
			ResultCardWidget->InitCardStatus(ResultCard);
		}
	}
	else
	{
		ResultCardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCraftWidget::Craft()
{
	if (FirstCardWidget->GetVisibility() == ESlateVisibility::Hidden || SecondCardWidget->GetVisibility() == ESlateVisibility::Hidden || ResultCardWidget->GetVisibility() == ESlateVisibility::Hidden) return;

	CardComponent->RemoveCardFromInventory(FirstCard);
	HideFirstCard();
	CardComponent->RemoveCardFromInventory(SecondCard);
	HideSecondCard();
	ResultCardWidget->SetVisibility(ESlateVisibility::Hidden);
	bool bIsValid;
	CardComponent->AddCardToInventory(CardComponent->GetCardCraftResult(FirstCard, SecondCard, bIsValid));

	MakeBackpack();
}

void UCraftWidget::HideFirstCard()
{
	FirstCardWidget->SetVisibility(ESlateVisibility::Hidden);
	CancleFirstCard();
	ShowCraftResult();
}

void UCraftWidget::HideSecondCard()
{
	SecondCardWidget->SetVisibility(ESlateVisibility::Hidden);
	CancleSecondCard();
	ShowCraftResult();
}

void UCraftWidget::CancleFirstCard()
{
	if (LastFirstCardWidget)
	{
		LastFirstCardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCraftWidget::CancleSecondCard()
{
	if (LastSecondCardWidget)
	{
		LastSecondCardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

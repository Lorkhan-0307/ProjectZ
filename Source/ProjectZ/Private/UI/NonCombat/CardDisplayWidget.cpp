// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombat/CardDisplayWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/Card/CardWidget.h"

void UCardDisplayWidget::SetDisplayCard(FCard Card)
{
	Card.IsValid = false;
	DisplayCard->InitCardStatus(Card, false);
	CardEffectText->SetText(Card.CardEffect);
	CardDescriptionText->SetText(Card.CardDescription);

	DisappearButton->OnClicked.AddDynamic(this, &UCardDisplayWidget::DestroyDisplayCard);
}

void UCardDisplayWidget::DestroyDisplayCard()
{
	RemoveFromParent();
	CollectGarbage(EObjectFlags::RF_BeginDestroyed);
}

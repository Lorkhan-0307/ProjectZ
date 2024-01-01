// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/NonCombat/NonCombatOverlay.h"

#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Ui/Card/CardWidget.h"
#include "Player/ZNonCombatPlayerController.h"
#include "UI/WidgetController/OverlayWidgetController.h"


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

void UNonCombatOverlay::WidgetControllerSet()
{
	Super::WidgetControllerSet();
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

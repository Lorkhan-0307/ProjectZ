// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/NonCombat/NonCombatOverlay.h"

#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "HUD/Card/CardWidget.h"
#include "Player/ZPlayerController.h"


void UNonCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
	
	if (CardComponent)
	{
		CardComponent->UpdateLeftHandCardDelegate.AddDynamic(this,&UNonCombatOverlay::UpdateLeftHandCard);
		CardComponent->UpdateRightHandCardDelegate.AddDynamic(this,&UNonCombatOverlay::UpdateRightHandCard);
	}
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
// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/NonCombat/NonCombatOverlay.h"

#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Ui/Card/CardWidget.h"
#include "Player/ZPlayerController.h"


void UNonCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Warning,TEXT("%d"),GetOwningPlayer()->GetCharacter()==nullptr?0:1);
	if (AZPlayerController* PlayerController = Cast<AZPlayerController>(GetOwningPlayer()))
	{
		if (AZCharacter* Character = Cast<AZCharacter>(PlayerController->GetCharacter()))
		{
			CardComponent = Character->GetCardComponent();
		}
	}
	
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
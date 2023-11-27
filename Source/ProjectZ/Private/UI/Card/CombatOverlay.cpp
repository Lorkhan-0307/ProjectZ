// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Card/CombatOverlay.h"
#include "UI/Card/CardHandWidget.h"
#include "Character/ZCharacter.h"
#include "Character/CardComponent.h"

void UCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	CardHandWidget->SetCardComponent(CardComponent);
	CardHandWidget->a=100;

	//CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
	// This Line has been moved to the Initoverlay function in the ZHUDbase code.
}



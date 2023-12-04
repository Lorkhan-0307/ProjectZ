// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardHandHUD.h"
#include "HUD/Card/CardHandWidget.h"
#include "Character/ZCharacter.h"
#include "Character/CardComponent.h"
#include "Character/ZCombatCharacter.h"

void UCardHandHUD::NativeConstruct()
{
	Super::NativeConstruct();
	CardHandWidget->SetViewportSize(ViewportSize);

	if(GetWorld()->GetName() == "L_FirstPersonMap") CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
	if(GetWorld()->GetName() == "L_CombatMap") CardComponent = Cast<AZCombatCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
}



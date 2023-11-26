// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardHandHUD.h"
#include "HUD/Card/CardHandWidget.h"
#include "Character/ZCharacter.h"
#include "Character/CardComponent.h"

void UCardHandHUD::NativeConstruct()
{
	Super::NativeConstruct();
	CardHandWidget->SetViewportSize(ViewportSize);

	CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
}



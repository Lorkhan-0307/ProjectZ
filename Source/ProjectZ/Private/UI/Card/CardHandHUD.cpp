// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Card/CardHandHUD.h"
#include "UI/Card/CardHandWidget.h"
#include "Character/ZCharacter.h"
#include "Character/CardComponent.h"

void UCardHandHUD::NativeConstruct()
{
	Super::NativeConstruct();
	CardHandWidget->SetViewportSize(ViewportSize);
	CardHandWidget->SetCardComponent(CardComponent);

	//CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
}



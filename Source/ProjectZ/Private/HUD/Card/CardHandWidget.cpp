// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardHandWidget.h"

#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Components/Overlay.h"
#include "HUD/Card/CardWidget.h"


void UCardHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
	CardComponent->DrawAndAddCardDelegate.AddDynamic(this,&UCardHandWidget::AddCardToHand);
}




void UCardHandWidget::AddCardToHand(FCard NewCard)
{
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	CardWidget->InitCardStatus(NewCard,HandCard.Num());
	CardOverlay->AddChild(CardWidget);
	CardWidget->AddToViewport();
	HandCard.Add(CardWidget);
}


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
	UE_LOG(LogTemp,Warning,TEXT("AAAA"));
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	CardWidget->InitCardStatus(NewCard,HandCard.Num());
	CardWidget->SetDesiredSizeInViewport(GetDesiredSize()*2);
	CardWidget->AddToViewport();
	CardOverlay->AddChild(CardWidget);
	HandCard.Add(CardWidget);
}


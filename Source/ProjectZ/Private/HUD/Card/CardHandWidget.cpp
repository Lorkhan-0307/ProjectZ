// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardHandWidget.h"

#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Components/Overlay.h"
#include "HUD/Card/CardWidget.h"
#include "HUD/ZHUD.h"


void UCardHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
	CardComponent->DrawAndAddCardDelegate.AddDynamic(this, &UCardHandWidget::AddCardToHand);
	CardSpawnPosition.Translation = FVector2D(1800, ViewportSize.Y);
	
}


void UCardHandWidget::AddCardToHand(FCard NewCard)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCenterPosition().X);
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	CardWidget->InitCardStatus(NewCard, HandCard.Num(), CardSpawnPosition);
	//CardWidget->RemoveFromParent();
	CardOverlay->AddChild(CardWidget);
	HandCard.Add(CardWidget);
	UpdateCardPosition();
	CardWidget->AddToViewport();
}

void UCardHandWidget::UpdateCardPosition()
{
	for (int i = 0; i < HandCard.Num(); i++)
	{
		HandCard[i]->DestinationTransform = CalculateCardPosition(i);
	}
}

FVector2D UCardHandWidget::GetCenterPosition()
{
	return ViewportSize / 2;
}

float UCardHandWidget::GetCardXPosition(int32 Index)
{
	//return GetCenterPosition().X + GetCardIndexPositionFromCenter(Index) * CardDistance;
	return 960 + GetCardIndexPositionFromCenter(Index) * CardDistance;
}

float UCardHandWidget::GetCardYPosition(int32 Index)
{
	return ViewportSize.Y;
}

float UCardHandWidget::GetCardIndexPositionFromCenter(int32 Index) const
{
	float NewIndex = (float)Index - (((float)HandCard.Num() - 1.f) / 2.f);
	if (NewIndex < 0)
	{
		NewIndex -= 0.5f;
	}
	NewIndex = FMath::RoundToFloat(NewIndex);
	if (HandCard.Num() % 2 == 0)
	{
		if (NewIndex < 0) NewIndex += 0.5f;
		else
		{
			NewIndex -= 0.5f;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%f"), NewIndex);
	return NewIndex;
}

FWidgetTransform UCardHandWidget::CalculateCardPosition(int32 Index)
{
	FWidgetTransform CardDestination;
	CardDestination.Translation.X = GetCardXPosition(Index);
	CardDestination.Translation.Y = GetCardYPosition(Index);
	return CardDestination;
}

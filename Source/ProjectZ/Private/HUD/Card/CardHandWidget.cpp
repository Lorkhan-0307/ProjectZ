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
	CardSpawnPosition.Translation = FVector2D(GetCachedGeometry().GetLocalSize());
	CenterPosition = GetCachedGeometry().GetLocalSize() / 2;
	UE_LOG(LogTemp, Warning, TEXT("Center %f"), CenterPosition.X);
}


void UCardHandWidget::AddCardToHand(FCard NewCard)
{
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	CardSpawnPosition.Translation = FVector2D(GetCachedGeometry().GetLocalSize());

	CardWidget->InitCardStatus(NewCard, HandCard.Num());
	CardWidget->SetRenderTransform(CardSpawnPosition);
	//CardWidget->RemoveFromParent();
	CardOverlay->AddChild(CardWidget);
	HandCard.Add(CardWidget);
	CardWidget->AddToViewport();
	UpdateCardPosition();
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
	//return 960 + GetCardIndexPositionFromCenter(Index) * CardDistance;
	return GetCachedGeometry().GetLocalSize().X / 2 + GetCardIndexPositionFromCenter(Index) * CardDistance - HandCard[Index]->GetCachedGeometry().GetLocalSize().X/2;
}

float UCardHandWidget::GetCardYPosition(int32 Index)
{
	//return ViewportSize.Y;
	return GetCachedGeometry().GetLocalSize().Y / 2;
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
	//CardDestination.Translation.Y = GetCardYPosition(Index);
	CardDestination.Translation.Y = 0;
	return CardDestination;
}

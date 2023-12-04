// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardHandWidget.h"

#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Character/ZCombatCharacter.h"
#include "Components/Overlay.h"
#include "HUD/Card/CardWidget.h"
#include "HUD/ZHUD.h"

void UCardHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(GetWorld()->GetName() == "L_FirstPersonMap") CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();
	if(GetWorld()->GetName() == "L_CombatMap") CardComponent = Cast<AZCombatCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();

	if (CardComponent)
	{
		CardComponent->DrawAndAddCardDelegate.AddDynamic(this, &UCardHandWidget::AddCardToHand);
		CardComponent->UpdateLeftHandCardDelegate.AddDynamic(this, &UCardHandWidget::UpdateLeftHandCard);
		CardComponent->UpdateRightHandCardDelegate.AddDynamic(this, &UCardHandWidget::UpdateRightHandCard);
	}
}

void UCardHandWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateCardPosition();
}


void UCardHandWidget::AddCardToHand(FCard NewCard)
{
	UCardWidget* CardWidget = CreateCardWidget(NewCard);
	HandCard.Add(CardWidget);
	CardWidget->SetRenderTransform(CardSpawnPosition);
}

void UCardHandWidget::UpdateCardPosition()
{
	for (int i = 0; i < HandCard.Num(); i++)
	{
		HandCard[i]->DestinationTransform = CalculateCardPosition(i);
	}
	if (LeftHandCardWidget)
		LeftHandCardWidget->DestinationTransform = CalculateCardPosition(-2);
	if (RightHandCardWidget)
		RightHandCardWidget->DestinationTransform = CalculateCardPosition(-1);
}

UCardWidget* UCardHandWidget::CreateCardWidget(FCard CardStatus)
{
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	CardWidget->InitCardStatus(CardStatus);
	CardWidget->CardHandWidget = this;

	CardWidget->RemoveFromParent();
	CardOverlay->AddChild(CardWidget);

	CardWidget->CardDragStartDelegate.AddDynamic(this, &UCardHandWidget::DragStarted);
	CardWidget->CardDragEndDelegate.AddDynamic(this, &UCardHandWidget::DragEnded);
	CardWidget->AddToViewport();
	return CardWidget;
}

float UCardHandWidget::GetCardXPosition(int32 Index)
{
	if (Index == -2)
	{
		return LeftHandCardPosition.X;
	}
	if (Index == -1)
	{
		return GetCachedGeometry().GetLocalSize().X - RightHandCardPosition.X - CardSize.X;
	}
	float XPos = GetCachedGeometry().GetLocalSize().X / 2 + GetCardIndexPositionFromCenter(Index) * CardDistance - CardSize.X / 2;
	if (HandCard[Index]->GetMouseHovered()) XPos += MouseHoveredHeight * FMath::Sin(FMath::DegreesToRadians(GetCardAngle(Index)));
	return XPos;
}

float UCardHandWidget::GetCardYPosition(int32 Index)
{
	if (Index == -2)
	{
		return LeftHandCardWidget->GetMouseHovered() ? -MouseHoveredHeight : 0;
	}
	if (Index == -1)
	{
		return RightHandCardWidget->GetMouseHovered() ? -MouseHoveredHeight : 0;
	}
	float YPos = FMath::Abs(GetCardIndexPositionFromCenter(Index)) * ArcHeight;
	if (HandCard[Index]->GetMouseHovered()) YPos -= MouseHoveredHeight * FMath::Cos(FMath::DegreesToRadians(GetCardAngle(Index)));
	return YPos;
}

float UCardHandWidget::GetCardAngle(int32 Index)
{
	return GetCardIndexPositionFromCenter(Index) * CardAngle;
}

void UCardHandWidget::DragStarted(UCardWidget* CardDragged)
{
	NowDragCard = CardDragged;
}

void UCardHandWidget::DragEnded(UCardWidget* CardDragged, bool bIsUsed)
{
	NowDragCard = nullptr;
	if (bIsUsed)
	{
		HandCard.Remove(CardDragged);
	}
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
	return NewIndex;
}

FWidgetTransform UCardHandWidget::CalculateCardPosition(int32 Index)
{
	FWidgetTransform CardDestination;
	CardDestination.Translation.X = GetCardXPosition(Index);
	CardDestination.Translation.Y = GetCardYPosition(Index);
	CardDestination.Angle = Index < 0 ? 0 : GetCardAngle(Index);
	return CardDestination;
}

void UCardHandWidget::UpdateLeftHandCard(FCard LeftCard)
{
	if (LeftHandCardWidget == nullptr)
	{
		LeftHandCardWidget = CreateCardWidget(LeftCard);
		LeftHandCardWidget->SetRenderTranslation(LeftHandCardPosition);
	}
	SetVisibility(ESlateVisibility::Visible);
	LeftHandCardWidget->InitCardStatus(LeftCard);
}

void UCardHandWidget::UpdateRightHandCard(FCard RightCard)
{
	if (RightHandCardWidget == nullptr)
	{
		RightHandCardWidget = CreateCardWidget(RightCard);
		RightHandCardWidget->SetRenderTranslation(RightHandCardPosition);
	}
	SetVisibility(ESlateVisibility::Visible);
	RightHandCardWidget->InitCardStatus(RightCard);
}

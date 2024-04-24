// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Card/CardHandWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "UI/Card/CardWidget.h"

void UCardHandWidget::SetCardComponent(UCardComponent* CC)
{
	CardComponent = CC;
	if (CardComponent)
	{
		CardComponent->DrawAndAddCardDelegate.AddDynamic(this, &UCardHandWidget::AddCardToHand);
	}
}

void UCardHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCardHandWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// TO DO : Find a way not to use tick
	UpdateCardPosition();
}

// Get Card, Create card widget
void UCardHandWidget::AddCardToHand(FCard NewCard)
{
	UCardWidget* CardWidget = CreateCardWidget(NewCard);
	HandCard.Add(CardWidget);
	CardWidget->SetRenderTransform(CardSpawnPosition);
}

// Set Card Position
void UCardHandWidget::UpdateCardPosition()
{
	for (int i = 0; i < HandCard.Num(); i++)
	{
		HandCard[i]->DestinationPosition = FVector2D(GetCardXPosition(i), GetCardYPosition(i));
		HandCard[i]->DestinationAngle = GetCardAngle(i);
	}
}

// Create Card Widget by FCard
UCardWidget* UCardHandWidget::CreateCardWidget(FCard CardStatus)
{
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	CardWidget->InitCardStatus(CardStatus);
	CardWidget->CardHandWidget = this;
	CardWidget->CardComponent = CardComponent;
	
	CardWidget->RemoveFromParent();
	CardCanvasPanel->AddChild(CardWidget);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CardWidget);
	CanvasPanelSlot->SetSize(FVector2D(200.f, 280.f));
	const FAnchors Anchors(0,0,0,0);
	CanvasPanelSlot->SetAnchors(Anchors);

	CardWidget->CardDragStartDelegate.AddDynamic(this, &UCardHandWidget::DragStarted);
	CardWidget->CardDragEndDelegate.AddDynamic(this, &UCardHandWidget::DragEnded);
	CardWidget->AddToViewport();
	return CardWidget;
}

float UCardHandWidget::GetCardXPosition(int32 Index)
{
	const UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HandCard[Index]);
	float XPos = CanvasPanelSlot->GetSize().X / 2 + GetCardIndexPositionFromCenter(Index) * CardDistance - CardSize.X / 2;
	if (HandCard[Index]->GetMouseHovered()) XPos += MouseHoveredHeight * FMath::Sin(FMath::DegreesToRadians(GetCardAngle(Index)));
	return XPos;
}

float UCardHandWidget::GetCardYPosition(int32 Index)
{
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

// Convert card index 0, 1, 2, 3, 4 -> -2, -1, 0, 1, 2
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

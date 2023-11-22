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

	CardSpawnPosition.Translation = FVector2D(1800, 0);
	//UE_LOG(LogTemp, Warning, TEXT("Center %f"), CenterPosition.X);
}

void UCardHandWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateCardPosition();
}


void UCardHandWidget::AddCardToHand(FCard NewCard)
{
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	HandCard.Add(CardWidget);
	CardWidget->InitCardStatus(NewCard, HandCard.Num());
	CardWidget->CardHandWidget = this;


	CardWidget->SetRenderTransform(CardSpawnPosition);
	CardWidget->RemoveFromParent();
	CardOverlay->AddChild(CardWidget);
	CardWidget->AddToViewport();

	CardWidget->CardDragStartDelegate.AddDynamic(this, &UCardHandWidget::DragStarted);
	CardWidget->CardDragEndDelegate.AddDynamic(this, &UCardHandWidget::DragEnded);
	CardWidget->MouseHoveredDelegate.AddDynamic(this, &UCardHandWidget::UpdateCardPosition);

	UpdateCardPosition();
}

void UCardHandWidget::UpdateCardPosition()
{
	for (int i = 0; i < HandCard.Num(); i++)
	{
		HandCard[i]->DestinationTransform = CalculateCardPosition(i);
		//UE_LOG(LogTemp, Warning, TEXT("i:%d X:%f"), i, CalculateCardPosition(i).Translation.X);
	}
}

FVector2D UCardHandWidget::GetCenterPosition()
{
	return ViewportSize / 2;
}

float UCardHandWidget::GetCardXPosition(int32 Index)
{
	float XPos = GetCachedGeometry().GetLocalSize().X / 2 + GetCardIndexPositionFromCenter(Index) * CardDistance - 100.f;
	if (HandCard[Index]->GetMouseHovered()) XPos += MouseHoveredHeight * FMath::Sin(FMath::DegreesToRadians(GetCardAngle(Index)));
	return XPos;
}

float UCardHandWidget::GetCardYPosition(int32 Index)
{
	float YPos = FMath::Abs(GetCardIndexPositionFromCenter(Index)) * ArcHeight;
	if (HandCard[Index]->GetMouseHovered())
	{
		//if (GetCardIndexPositionFromCenter(Index) > 0) YPos += MouseHoveredHeight * FMath::Cos(GetCardAngle(Index));
		YPos -= MouseHoveredHeight * FMath::Cos(FMath::DegreesToRadians(GetCardAngle(Index)));
	}

	return YPos;
}

float UCardHandWidget::GetCardAngle(int32 Index)
{
	return GetCardIndexPositionFromCenter(Index) * CardAngle;
}

void UCardHandWidget::DragStarted(UCardWidget* CardDragged)
{
	UE_LOG(LogTemp, Warning, TEXT("DragStart"));
	NowDragCard = CardDragged;
}

void UCardHandWidget::DragEnded(UCardWidget* CardDragged, bool bIsUsed)
{
	UE_LOG(LogTemp, Warning, TEXT("DragEnd"));
	NowDragCard = nullptr;
	if (bIsUsed)
	{
		HandCard.Remove(CardDragged);
		UpdateCardPosition();
	}
	for (auto i : HandCard)
	{
		UE_LOG(LogTemp, Warning, TEXT("%p"), i->CardName);
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
	CardDestination.Angle = GetCardAngle(Index);
	return CardDestination;
}

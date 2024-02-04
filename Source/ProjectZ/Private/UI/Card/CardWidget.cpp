// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Card/CardWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Card.h"
#include "Engine/DataTable.h"
// TO DO : Replace to CombatPlayerController
#include "Player/ZNonCombatPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Card/CardDragDropOperation.h"
#include "Ui/Card/CardHandWidget.h"


void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetDesiredSizeInViewport(CardSize);
}

void UCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this))
	{
		if (CanvasPanelSlot->GetPosition() != DestinationPosition) SetPosition(InDeltaTime);
	}
}

void UCardWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	bMouseHovered = true;
}


void UCardWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bMouseHovered = false;
}

FReply UCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UCardDragDropOperation* CardDragDropOperation = NewObject<UCardDragDropOperation>();

	CardDragDropOperation->Payload = this;
	UCardWidget* DragCardWidget = Cast<UCardWidget>(CreateWidget(GetOwningPlayer(), CardWidgetClass));
	DragCardWidget->InitCardStatus(CardStat);
	CardDragDropOperation->DefaultDragVisual = DragCardWidget;
	CardDragDropOperation->Pivot = EDragPivot::CenterCenter;
	SetVisibility(ESlateVisibility::Hidden);

	OutOperation = CardDragDropOperation;
	CardDragStartDelegate.Broadcast(this);
}

void UCardWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	//const UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InOperation->DefaultDragVisual);
	UE_LOG(LogTemp, Warning, TEXT("%d"), GEngine->GameViewport->Viewport->GetSizeXY().Y);
	if (InOperation->DefaultDragVisual->GetCachedGeometry().AbsolutePosition.Y < GEngine->GameViewport->Viewport->GetSizeXY().Y * CardComponent->GetPlayCardHeight())
	{
		// TO DO : Active Card Effect
		CardComponent->ActiveCard(CardStat);

		CardDragEndDelegate.Broadcast(this, true);
		RemoveFromParent();
		CollectGarbage(EObjectFlags::RF_BeginDestroyed);
	}
	else
	{
		CardDragEndDelegate.Broadcast(this, false);
		SetVisibility(ESlateVisibility::Visible);
		bMouseHovered = false;
	}
}

// Initialize Card UI by FCard
void UCardWidget::InitCardStatus(FCard CardStatus)
{
	CardStat = CardStatus;
	CardName->SetText(CardStatus.CardName);
	CardImage->SetBrushFromTexture(CardStatus.CardImage);
	CardDescription->SetText(CardStatus.CardDescription);
	ManaText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
	AtkText->SetText(FText::FromString(FString::FromInt(CardStatus.CardAtk)));
	DefText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
}

// Set Position by Interpolation
void UCardWidget::SetPosition(float DeltaTime)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (CanvasPanelSlot == nullptr) return;
	CanvasPanelSlot->SetPosition(FMath::Vector2DInterpTo(CanvasPanelSlot->GetPosition(), DestinationPosition, DeltaTime, InterpSpeed));
	SetRenderTransformAngle(FMath::FInterpTo(GetRenderTransform().Angle, DestinationAngle, DeltaTime, InterpSpeed));
}

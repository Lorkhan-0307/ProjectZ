// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Card.h"
#include "Engine/DataTable.h"
#include "Player/ZPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "HUD/Card/CardDragDropOperation.h"
#include "HUD/Card/CardHandWidget.h"


void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//InitCardStatus();

	//if (GEngine) GEngine->GameViewport->GetViewportSize(ViewportSize);
	CardSize = GetCachedGeometry().GetLocalSize();

	CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();

	//CardComponent->UpdateHandCardDelegate.AddDynamic(this, &UCardWidget::UpdateCardDestination);
}

void UCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (GetCachedGeometry().GetAbsolutePosition().X != DestinationTransform.Translation.X) SetPosition(InDeltaTime);
}

void UCardWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	bMouseHovered = true;
	//MouseHoveredDelegate.Broadcast();
}


void UCardWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bMouseHovered = false;
	//MouseHoveredDelegate.Broadcast();
}

FReply UCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	CardDragStartDelegate.Broadcast(this);
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	UCardDragDropOperation* CardDragDropOperation = NewObject<UCardDragDropOperation>();

	CardDragDropOperation->Payload = this;
	UCardWidget* DragCardWidget = Cast<UCardWidget>(CreateWidget(GetOwningPlayer(), CardWidgetClass));
	DragCardWidget->InitCardStatus(CardStat, CardIndex);
	CardDragDropOperation->DefaultDragVisual = DragCardWidget;
	CardDragDropOperation->Pivot = EDragPivot::CenterCenter;
	SetVisibility(ESlateVisibility::Hidden);


	OutOperation = CardDragDropOperation;
	CardDragStartDelegate.Broadcast(this);
	CardHandWidget->DragStarted(this);
}

void UCardWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	if (InOperation->DefaultDragVisual->GetCachedGeometry().GetAbsolutePosition().Y < 300)
	{
		// TO DO : Active Card Effect
		CardDragEndDelegate.Broadcast(this, true);
		CardHandWidget->DragEnded(this,true);
		RemoveFromParent();
		CollectGarbage(EObjectFlags::RF_BeginDestroyed);
	}
	else
	{
		CardDragEndDelegate.Broadcast(this, false);
		CardHandWidget->DragEnded(this,false);
		SetVisibility(ESlateVisibility::Visible);
		bMouseHovered = false;
	}
}

void UCardWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	CardDragStartDelegate.Broadcast(this);
}

void UCardWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	CardDragEndDelegate.Broadcast(this, false);
}


void UCardWidget::InitCardStatus(FCard CardStatus, int32 Index)
{
	CardStat = CardStatus;
	CardName->SetText(CardStatus.CardName);
	CardImage->SetBrushFromTexture(CardStatus.CardImage);
	CardDescription->SetText(CardStatus.CardDescription);
	ManaText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
	AtkText->SetText(FText::FromString(FString::FromInt(CardStatus.CardAtk)));
	DefText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
	CardIndex = Index;
	//SetRenderTranslation(WidgetTransform.Translation);
	//DestinationTransform = WidgetTransform;
	//UpdateCardDestination(CardIndex);
}

void UCardWidget::SetPosition(float DeltaTime)
{
	if (GetRenderTransform() == DestinationTransform) return;
	SetRenderTranslation(FMath::Vector2DInterpTo(GetRenderTransform().Translation, DestinationTransform.Translation, DeltaTime, InterpSpeed));
	SetRenderTransformAngle(FMath::FInterpTo(GetRenderTransform().Angle,DestinationTransform.Angle,DeltaTime,InterpSpeed));
}

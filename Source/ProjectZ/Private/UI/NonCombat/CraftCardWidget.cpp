// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombat/CraftCardWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Card/CardDragDropOperation.h"
#include "UI/HUD/ZHUDBase.h"
#include "UI/NonCombat/CraftWidget.h"
#include "UI/NonCombat/InventoryWidget.h"

void UCraftCardWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UCraftCardWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UCraftCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UCraftCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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
}

void UCraftCardWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);

	if (CraftWidget == nullptr)
	{
		AZHUDBase* HUD = Cast<AZHUDBase>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		CraftWidget = Cast<UInventoryWidget>(HUD->InventoryWidget)->CraftWidget;
		FirstCardWidgetLocMin = CraftWidget->FirstCardBackGroundImage->GetCachedGeometry().GetAbsolutePosition();
		FirstCardWidgetLocMax = FirstCardWidgetLocMin + CraftWidget->FirstCardBackGroundImage->GetCachedGeometry().GetAbsoluteSize();
		SecondCardWidgetLocMin = CraftWidget->SecondCardBackGroundImage->GetCachedGeometry().GetAbsolutePosition();
		SecondCardWidgetLocMax = SecondCardWidgetLocMin + CraftWidget->SecondCardBackGroundImage->GetCachedGeometry().GetAbsoluteSize();
	}

	FVector2D WidgetPos = InOperation->DefaultDragVisual->GetCachedGeometry().GetAbsolutePosition() + InOperation->DefaultDragVisual->GetCachedGeometry().GetAbsoluteSize() / 2.f;

	if (FirstCardWidgetLocMin.ComponentwiseAllLessOrEqual(WidgetPos) && WidgetPos.ComponentwiseAllLessOrEqual(FirstCardWidgetLocMax))
	{
		CraftWidget->SetFirstCardWidget(CardStat,this);
		SetVisibility(ESlateVisibility::Hidden);

		return;
	}

	if (SecondCardWidgetLocMin.ComponentwiseAllLessOrEqual(WidgetPos) && WidgetPos.ComponentwiseAllLessOrEqual(SecondCardWidgetLocMax))
	{
		CraftWidget->SetSecondCardWidget(CardStat, this);
		SetVisibility(ESlateVisibility::Hidden);

		return;
	}

	SetVisibility(ESlateVisibility::Visible);
}

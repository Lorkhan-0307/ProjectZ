// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Card/CardWidget.h"
#include "CraftCardWidget.generated.h"

class UCraftWidget;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UCraftCardWidget : public UCardWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	UPROPERTY()
	UCraftWidget* CraftWidget;

	FVector2D FirstCardWidgetLocMin;
	FVector2D FirstCardWidgetLocMax;
	FVector2D SecondCardWidgetLocMin;
	FVector2D SecondCardWidgetLocMax;
};

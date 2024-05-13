// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Card/CardWidget.h"
#include "CombatCardWidget.generated.h"

class UCanvasPanelSlot;
class UTextBlock;
class UImage;
class UDataTable;
class UCardComponent;
class UCardHandWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMouseHoveredDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardDragStartDelegate, UCombatCardWidget*, DragCard);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardDragEndDelegate, UCombatCardWidget*, DragCard, bool, bIsUsed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickCardWidgetDelegate, FCard, ClickCard);

UCLASS()
class PROJECTZ_API UCombatCardWidget : public UCardWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// Where the card should be
	UPROPERTY(BlueprintReadWrite)
	FVector2D DestinationPosition;

	float DestinationAngle;

	UPROPERTY()
	UCardHandWidget* CardHandWidget;

	FCardDragStartDelegate CardDragStartDelegate;
	FCardDragEndDelegate CardDragEndDelegate;

	FClickCardWidgetDelegate* ClickCardWidgetDelegate;

	virtual void InitCardStatus(FCard CardStatus, bool bSetDelegate = true);

	FColor HoveredColor = FColor(255, 43, 43);

	UFUNCTION()
	void DestroyActivateCard();

	UFUNCTION()
	void CancelActivateCard();

	FORCEINLINE bool GetMouseHovered() const { return bMouseHovered; }

protected:
	// Card Move Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	int32 InterpSpeed = 5;

	void SetPosition(float DeltaTime);

private:
	bool bIsInPosition = false;

	FVector2D ViewportSize;

	bool bMouseHovered = false;
	
	float CardHandYSize;

	bool bTrashCard = false;

	UPROPERTY()
	UCanvasPanelSlot* CanvasPanelSlot;

	FVector2D TrashCardStartPosition;

	void TrashCard(float DeltaTime);
};

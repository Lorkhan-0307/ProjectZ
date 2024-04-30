// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "Data/Card.h"
#include "CardWidget.generated.h"

/**
 * 
 */

class UCanvasPanelSlot;
class UTextBlock;
class UImage;
class UDataTable;
class UCardComponent;
class UCardHandWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMouseHoveredDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardDragStartDelegate, UCardWidget*, DragCard);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardDragEndDelegate, UCardWidget*, DragCard, bool, bIsUsed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickCardWidgetDelegate, FCard, ClickCard);

UCLASS()
class PROJECTZ_API UCardWidget : public UUserWidget
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* CardImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CardDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ManaText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AtkText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DefText;

	// Where the card should be
	UPROPERTY(BlueprintReadWrite)
	FVector2D DestinationPosition;

	float DestinationAngle;

	UPROPERTY()
	UCardHandWidget* CardHandWidget;

	UPROPERTY()
	UCardComponent* CardComponent;

	FCardDragStartDelegate CardDragStartDelegate;
	FCardDragEndDelegate CardDragEndDelegate;

	FClickCardWidgetDelegate* ClickCardWidgetDelegate;

	void InitCardStatus(FCard CardStatus, bool bSetDelegate = true);

	UFUNCTION()
	void DestroyActivateCard();

	UFUNCTION()
	void CancelActivateCard();
	
	void DestroyWidget();

	FORCEINLINE bool GetMouseHovered() const { return bMouseHovered; }
	FORCEINLINE FCard GetCardStat() const { return CardStat; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	UDataTable* CardDataTable;

	// Card Move Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	int32 InterpSpeed = 5;

	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D CardSize = FVector2D(200.f, 280.f);

	void SetPosition(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Card)
	TSubclassOf<UCardWidget> CardWidgetClass;

private:
	bool bIsInPosition = false;

	FVector2D ViewportSize;

	bool bMouseHovered = false;

	FCard CardStat;

	float CardHandYSize;

	bool bTrashCard = false;

	UPROPERTY()
	UCanvasPanelSlot* CanvasPanelSlot;

	FVector2D TrashCardStartPosition;

	void TrashCard(float DeltaTime);
};

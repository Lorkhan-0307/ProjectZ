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

class UTextBlock;
class UImage;
class UDataTable;
class UCardComponent;
class UCardHandWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMouseHoveredDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardDragStartDelegate, UCardWidget*, DragCard);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardDragEndDelegate, UCardWidget*, DragCard, bool, bIsUsed);

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

	UPROPERTY(BlueprintReadWrite)
	FWidgetTransform DestinationTransform;

	UPROPERTY()
	UCardHandWidget* CardHandWidget;
	
	UPROPERTY()
	UCardComponent* CardComponent;

	FCardDragStartDelegate CardDragStartDelegate;
	FCardDragEndDelegate CardDragEndDelegate;

	void InitCardStatus(FCard CardStatus);

	FORCEINLINE void SetCardIndex(int32 Index) { CardIndex = Index; }

	FORCEINLINE bool GetMouseHovered() const { return bMouseHovered; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	UDataTable* CardDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	int32 InterpSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	int32 CardDistance = 110;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	float UseCardHeight = 300;

	void SetPosition(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Card)
	TSubclassOf<UCardWidget> CardWidgetClass;

private:
	bool bIsInPosition = false;

	int32 CardIndex = 0;

	FVector2D ViewportSize;


	bool bMouseHovered = false;

	FCard CardStat;
};

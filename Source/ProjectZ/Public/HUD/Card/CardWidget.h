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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardDragStartDelegate,UCardWidget*,DragCard);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardDragEndDelegate,UCardWidget*,DragCard,bool,bIsUsed);

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
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	//void InitialCard(FCard CardStatus);

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

	FMouseHoveredDelegate MouseHoveredDelegate;
	FCardDragStartDelegate CardDragStartDelegate;
	FCardDragEndDelegate CardDragEndDelegate;

	UCardHandWidget* CardHandWidget;

	void InitCardStatus(FCard CardStatus, int32 Index);

	void SetCardIndex(int32 Index) { CardIndex = Index; }

	FORCEINLINE bool GetMouseHovered() const { return bMouseHovered; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* CardDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InterpSpeed = 5;

	UPROPERTY(EditAnywhere)
	int32 CardDistance = 110;

	UPROPERTY(EditAnywhere)
	float UseCardHeight = 300;

	//UDragD

	void SetPosition(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Card)
	TSubclassOf<UCardWidget> CardWidgetClass;


private:
	bool bIsInPosition = false;

	int32 CardIndex = 0;

	UPROPERTY()
	UCardComponent* CardComponent;

	FVector2D ViewportSize;

	FVector2D CardSize;

	bool bMouseHovered = false;

	FCard CardStat;
};

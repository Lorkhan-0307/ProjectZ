// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardWidget.h"
#include "Blueprint/UserWidget.h"
#include "Data/Card.h"
#include "CardHandWidget.generated.h"

class UOverlay;
class UCardComponent;

UCLASS()
class PROJECTZ_API UCardHandWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetViewportSize(const FVector2D& Size) { ViewportSize = Size; }

	void DragStarted(UCardWidget* CardDragged);
	void DragEnded(UCardWidget* CardDragged, bool bIsUsed);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* CardOverlay;

	UPROPERTY(EditAnywhere, Category = "Card")
	TSubclassOf<UCardWidget> CardWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
	TArray<UCardWidget*> HandCard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
	FCard LeftHandCard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
	FCard RightHandCard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	float CardDistance = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	float MouseHoveredHeight = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	float CardAngle = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	float ArcHeight = 10.f;

	float GetCardIndexPositionFromCenter(int32 Index) const;

	FWidgetTransform CalculateCardPosition(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Card")
	FWidgetTransform CardSpawnPosition;

	UFUNCTION(BlueprintCallable)
	void AddCardToHand(FCard NewCard);

	void UpdateCardPosition();

private:
	UPROPERTY()
	UCardComponent* CardComponent;

	FVector2D ViewportSize;

	FVector2D GetCenterPosition();

	UPROPERTY()
	UCardWidget* NowDragCard;

	float GetCardXPosition(int32 Index);
	float GetCardYPosition(int32 Index);
	float GetCardAngle(int32 Index);
	
};

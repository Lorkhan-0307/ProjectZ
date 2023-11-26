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
	void SetCardComponent(UCardComponent* CC);
	FORCEINLINE void SetViewportSize(const FVector2D& Size) { ViewportSize = Size; }
	FORCEINLINE float GetPlayCardHeight() const { return PlayCardHeight; }
	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* CardOverlay;

	UPROPERTY(EditAnywhere, Category = Card)
	TSubclassOf<UCardWidget> CardWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Card)
	TArray<UCardWidget*> HandCard;

	UPROPERTY()
	UCardWidget* LeftHandCardWidget;

	UPROPERTY()
	UCardWidget* RightHandCardWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	float CardDistance = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	float MouseHoveredHeight = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	float CardAngle = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	float ArcHeight = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	float PlayCardHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	FVector2D CardSize = FVector2D(200, 300);

	float GetCardIndexPositionFromCenter(int32 Index) const;

	FWidgetTransform CalculateCardPosition(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	FWidgetTransform CardSpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	FVector2D LeftHandCardPosition; // Distance from the Left

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	FVector2D RightHandCardPosition; // Distance from the Right

	UFUNCTION(BlueprintCallable)
	void AddCardToHand(FCard NewCard);

	void UpdateCardPosition();

private:
	UPROPERTY()
	UCardComponent* CardComponent;

	FVector2D ViewportSize;

	UPROPERTY()
	UCardWidget* NowDragCard;

	UCardWidget* CreateCardWidget(FCard CardStatus);

	float GetCardXPosition(int32 Index);
	float GetCardYPosition(int32 Index);
	float GetCardAngle(int32 Index);

	UFUNCTION()
	void UpdateLeftHandCard(FCard LeftCard);

	UFUNCTION()
	void UpdateRightHandCard(FCard RightCard);

	UFUNCTION()
	void DragStarted(UCardWidget* CardDragged);

	UFUNCTION()
	void DragEnded(UCardWidget* CardDragged, bool bIsUsed);
};

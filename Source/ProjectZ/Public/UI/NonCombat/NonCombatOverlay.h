// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "NonCombatOverlay.generated.h"

class UTextBlock;
class UCanvasPanel;
class UOverlay;
class UCardHandWidget;
class USizeBox;
class UProgressBar;
class UCardWidget;
class UCardComponent;
class UImage;

UCLASS()
class PROJECTZ_API UNonCombatOverlay : public UZUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void SetCardComponent(UCardComponent* CC) override;

protected:
	virtual void WidgetControllerSet() override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MentalityBar;

	UPROPERTY(meta = (BindWidget))
	UImage* HealthCheckingBar;

	UPROPERTY(meta = (BindWidget))
	UImage* MentalityCheckingBar;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCardWidget> CardWidgetClass;

	//UPROPERTY(meta = (BindWidget))
	//UCardHandWidget* CardHandWidget;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* LeftHandCardWidget;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* RightHandCardWidget;

	UPROPERTY(meta = (BindWidget))
	USizeBox* CardHandSizeBox;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CardHandCanvas;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnText;

	float Health;
	float MaxHealth;
	float Mentality;
	float MaxMentality;

	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<UCardWidget*> HandCard;

	// Card Horizontal Distance
	UPROPERTY(EditAnywhere, Category = Card)
	float CardDistance = 150.f;

	// The height that goes up when you put the mouse on the card
	UPROPERTY(EditAnywhere, Category = Card)
	float MouseHoveredHeight = 50.f;

	// Angle between cards
	UPROPERTY(EditAnywhere, Category = Card)
	float CardAngle = 10.f;

	// Card Vertical Distance
	UPROPERTY(EditAnywhere, Category = Card)
	float ArcHeight = 10.f;

	// Height when use the card
	UPROPERTY(EditAnywhere, Category = Card)
	float PlayCardHeight = 300.f;

	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D CardSize = FVector2D(200, 300);

	// Health, Mentality
	UFUNCTION()
	virtual void OnHealthChanged(float NewValue);

	UFUNCTION()
	virtual void OnMaxHealthChanged(float NewValue);

	UFUNCTION()
	virtual void OnMentalityChanged(float NewValue);

	UFUNCTION()
	virtual void OnMaxMentalityChanged(float NewValue);

	float GetHealthCheckingBarPos();
	float GetMentalityCheckingBarPos();
	void SetHealthCheckingBarPos();
	void SetMentalityCheckingBarPos();

	UFUNCTION()
	void UpdateLeftHandCard(FCard LeftCard);

	UFUNCTION()
	void UpdateRightHandCard(FCard RightCard);

	float GetCardIndexPositionFromCenter(int32 Index) const;

	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D CardSpawnPosition = FVector2D(1800.f,0.f);

	UFUNCTION(BlueprintCallable)
	void AddCardToHand(FCard NewCard);

	void UpdateCardPosition();

	UPROPERTY()
	UCardWidget* NowDragCard;

	UCardWidget* CreateCardWidget(FCard CardStatus);

	float GetCardXPosition(int32 Index);
	float GetCardYPosition(int32 Index);
	float GetCardAngle(int32 Index);

	UFUNCTION()
	void DragStarted(UCardWidget* CardDragged);

	UFUNCTION()
	void DragEnded(UCardWidget* CardDragged, bool bIsUsed);

	UFUNCTION()
	void TurnChanged(ETurn Turn);

	void SetCardHandPosition(float DeltaTime);

	ETurn CurrentTurn;

	bool bCardHandPositionSet = false;

	UPROPERTY(EditAnywhere, Category = Card)
	float CardHandInterpSpeed = 1.f;

	UFUNCTION(BlueprintCallable)
	void ShowTurnText();

	UFUNCTION()
	void HideTurnText();

	FTimerHandle TurnTextTimer;

	UPROPERTY(EditAnywhere)
	float TurnTextDisplayTime = 3.f;
};

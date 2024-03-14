// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "NonCombatOverlay.generated.h"

class UTurnChangeWidget;
class AZGameModeBase;
class UCharacterPortraitWidget;
class UButton;
class UCostPathLengthWidget;
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
	UCanvasPanel* OverlayCanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MentalityBar;

	UPROPERTY(meta = (BindWidget))
	UImage* HealthCheckingBar;

	UPROPERTY(meta = (BindWidget))
	UImage* MentalityCheckingBar;

	UPROPERTY(meta = (BindWidget))
	UOverlay* CostOverlay;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CostText;

	UPROPERTY(meta = (BindWidget))
	UCostPathLengthWidget* PathLengthCostWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCardWidget> CardWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCharacterPortraitWidget> CharacterPortraitWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTurnChangeWidget> TurnChangeWidgetClass;

	//UPROPERTY(meta = (BindWidget))
	//UCardHandWidget* CardHandWidget;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* LeftHandCardWidget;

	UPROPERTY(meta = (BindWidget))
	USizeBox* LeftHandSizeBox;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* RightHandCardWidget;

	UPROPERTY(meta = (BindWidget))
	USizeBox* RightHandSizeBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* CardHandSizeBox;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CardHandCanvas;

	UPROPERTY(meta = (BindWidget))
	UOverlay* ShowSkillCardOverlay;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* ShowSkillCardWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* TurnEndButton;

	UPROPERTY(meta = (BindWidget))
	USizeBox* DiscardCardSizeBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* CardGraveyardSizeBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* CharacterPortraitSizeBox;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CharacterPortraitCanvasPanel;

	UPROPERTY()
	UTurnChangeWidget* TurnChangeWidget;

	UPROPERTY()
	AZGameModeBase* GameMode;

	float Health;
	float MaxHealth;
	float Mentality;
	float MaxMentality;
	float Cost;
	float MaxCost;

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

	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D ShowSKillCardLocation = FVector2D(1500.f, 300.f);

	// Health, Mentality
	UFUNCTION()
	virtual void OnHealthChanged(float NewValue);

	UFUNCTION()
	virtual void OnMaxHealthChanged(float NewValue);

	UFUNCTION()
	virtual void OnMentalityChanged(float NewValue);

	UFUNCTION()
	virtual void OnMaxMentalityChanged(float NewValue);

	UFUNCTION()
	void OnCostChanged(float NewValue);

	UFUNCTION()
	void OnMaxCostChanged(float NewValue);

	float GetHealthCheckingBarPos();
	float GetMentalityCheckingBarPos();
	void SetHealthCheckingBarPos();
	void SetMentalityCheckingBarPos();

	UFUNCTION()
	void UpdateLeftHandCard(FCard LeftCard);

	UFUNCTION()
	void UpdateRightHandCard(FCard RightCard);

	float GetIndexPositionFromCenter(int32 Index, int32 Size) const;

	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D CardSpawnPosition = FVector2D(1800.f, 0.f);

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

	UPROPERTY()
	TArray<UCharacterPortraitWidget*> PortraitWidgets;

	UPROPERTY(EditAnywhere, Category = Card)
	float CardHandInterpSpeed = 1.f;

	void SetCostPathLengthWidgetPosition();

	void ShowCostWidget(bool bShow);

	void ShowTurnEndButton(bool bShow);

	UFUNCTION()
	void ShowSkillCard(FCard Card);

	UFUNCTION()
	void HideSkillCard();

	float CharacterPortraitCenter;

	UFUNCTION()
	void ShowCharacterPortrait();
	
	void HideCharacterPortrait();
	void UpdateCharacterPortrait();

	UFUNCTION()
	void DiscardCharacterPortrait(AActor* Actor);

	UFUNCTION()
	void TurnEnd();
};


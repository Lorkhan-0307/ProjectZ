// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftCardWidget.h"
#include "Blueprint/UserWidget.h"
#include "Data/Card.h"
#include "CraftWidget.generated.h"

class UCraftCardWidget;
class UBackpackWidget;
class UCardComponent;
class UCardWidget;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UCraftWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CraftButton;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* FirstCardWidget;

	UPROPERTY(meta = (BindWidget))
	UImage* FirstCardBackGroundImage;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* SecondCardWidget;

	UPROPERTY(meta = (BindWidget))
	UImage* SecondCardBackGroundImage;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* ResultCardWidget;

	UPROPERTY(meta = (BindWidget))
	UBackpackWidget* BackpackWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* FirstCardWidgetButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SecondCardWidgetButton;

	UPROPERTY()
	UCardComponent* CardComponent;

	UPROPERTY()
	UCraftCardWidget* LastFirstCardWidget;
	
	UPROPERTY()
	UCraftCardWidget* LastSecondCardWidget;

	void MakeBackpack();

	void SetFirstCardWidget(FCard& CardStatus, UCraftCardWidget* CraftCardWidget);

	void SetSecondCardWidget(FCard& CardStatus, UCraftCardWidget* CraftCardWidget);

	UFUNCTION()
	void HideFirstCard();

	UFUNCTION()
	void HideSecondCard();
	
	void CancleFirstCard();
	
	void CancleSecondCard();

private:
	FCard FirstCard;
	FCard SecondCard;
	
	void ShowCraftResult();

	UFUNCTION()
	void Craft();
};

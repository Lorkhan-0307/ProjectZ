// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Card.h"
#include "CardDisplayWidget.generated.h"

class UButton;
class UTextBlock;
class UCardWidget;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UCardDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UCardWidget* DisplayCard;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CardEffectText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CardDescriptionText;

	UPROPERTY(meta = (BindWidget))
	UButton* DisappearButton;

	void SetDisplayCard(FCard Card);

	UFUNCTION()
	void DestroyDisplayCard();
};

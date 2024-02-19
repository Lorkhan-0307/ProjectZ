// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "DialogWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UDialogWidget : public UZUserWidget
{
	GENERATED_BODY()

protected:
	virtual void WidgetControllerSet() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogText;

	UPROPERTY(meta = (BindWidget))
	UButton* Select1Button;

	UPROPERTY(meta = (BindWidget))
	UButton* Select2Button;

	UPROPERTY(meta = (BindWidget))
	UButton* Select3Button;

	UPROPERTY(meta = (BindWidget))
	UImage* Select1Image;

	UPROPERTY(meta = (BindWidget))
	UImage* Select2Image;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Select3Image;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Select1Text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Select2Text;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Select3Text;

private:
	UFUNCTION()
	void Select1ButtonPressed();

	UFUNCTION()
	void Select2ButtonPressed();

	UFUNCTION()
	void Select3ButtonPressed();
	
};

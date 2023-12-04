// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "CombatOverlay.generated.h"

class UCardWidget;
class UCardHandWidget;
class UCardComponent;
class UProgressBar;

/**
 * 
 */

// It was CardHandHUD
UCLASS()
class PROJECTZ_API UCombatOverlay : public UZUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCardHandWidget* CardHandWidget;

	/*
	virtual void OnHealthChanged(float NewValue) override;
	
	virtual void OnMaxHealthChanged(float NewValue) override; 
	
	virtual void OnMentalityChanged(float NewValue) override;
	
	virtual void OnMaxMentalityChanged(float NewValue) override;
	*/

	virtual void WidgetControllerSet() override;



private:
};

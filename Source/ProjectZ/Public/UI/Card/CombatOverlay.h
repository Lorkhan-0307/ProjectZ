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
	

	virtual void WidgetControllerSet() override;



private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MentalityBar;
	
	UFUNCTION()
	virtual void OnHealthChanged(float NewValue);
	
	UFUNCTION()
	virtual void OnMaxHealthChanged(float NewValue); 

	UFUNCTION()
	virtual void OnMentalityChanged(float NewValue);

	UFUNCTION()
	virtual void OnMaxMentalityChanged(float NewValue);
};

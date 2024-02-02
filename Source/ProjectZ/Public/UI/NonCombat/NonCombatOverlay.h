// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "NonCombatOverlay.generated.h"

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
	virtual void SetCardComponent(UCardComponent* CC) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCardWidget> CardWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* LeftHandCardWidget;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* RightHandCardWidget;

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
	
	
	UFUNCTION()
	virtual void OnHealthChanged(float NewValue);
	
	UFUNCTION()
	virtual void OnMaxHealthChanged(float NewValue); 

	UFUNCTION()
	virtual void OnMentalityChanged(float NewValue);

	UFUNCTION()
	virtual void OnMaxMentalityChanged(float NewValue);
	
	
	UFUNCTION()
	void UpdateLeftHandCard(FCard LeftCard);

	UFUNCTION()
	void UpdateRightHandCard(FCard RightCard);

	float GetHealthCheckingBarPos();
	float GetMentalityCheckingBarPos();
	void SetHealthCheckingBarPos();
	void SetMentalityCheckingBarPos();
};

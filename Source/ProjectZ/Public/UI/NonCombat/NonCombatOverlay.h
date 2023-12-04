// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "NonCombatOverlay.generated.h"

class UProgressBar;
class UCardWidget;
class UCardComponent;

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

	/*
	virtual void OnHealthChanged(float NewValue) override;
	
	virtual void OnMaxHealthChanged(float NewValue) override; 
	
	virtual void OnMentalityChanged(float NewValue) override;
	
	virtual void OnMaxMentalityChanged(float NewValue) override;
	*/
	
	
	UFUNCTION()
	void UpdateLeftHandCard(FCard LeftCard);

	UFUNCTION()
	void UpdateRightHandCard(FCard RightCard);
};

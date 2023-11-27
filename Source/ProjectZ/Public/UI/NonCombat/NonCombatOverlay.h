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
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MentalityBar;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* LeftHandCardWidget;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* RightHandCardWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCardWidget> CardWidgetClass;

	virtual void WidgetControllerSet() override;

private:
	float Health;
	float MaxHealth;
	
	UFUNCTION()
	void UpdateHealth(float NewValue);

	UFUNCTION()
	void UpdateMaxHealth(float NewValue);

	UFUNCTION()
	void UpdateMentality(float NewValue);
	
	UFUNCTION()
	void UpdateLeftHandCard(FCard LeftCard);

	UFUNCTION()
	void UpdateRightHandCard(FCard RightCard);
};

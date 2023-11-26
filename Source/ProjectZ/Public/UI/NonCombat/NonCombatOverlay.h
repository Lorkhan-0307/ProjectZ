// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "NonCombatOverlay.generated.h"

class UCardWidget;
class UCardComponent;

UCLASS()
class PROJECTZ_API UNonCombatOverlay : public UZUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	UCardWidget* LeftHandCardWidget;

	UPROPERTY(meta = (BindWidget))
	UCardWidget* RightHandCardWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCardWidget> CardWidgetClass;

private:
	UFUNCTION()
	void UpdateLeftHandCard(FCard LeftCard);

	UFUNCTION()
	void UpdateRightHandCard(FCard RightCard);
};


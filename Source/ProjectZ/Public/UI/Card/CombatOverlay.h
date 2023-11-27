// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "CardHandHUD.generated.h"

class UCardWidget;
class UCardHandWidget;
class UCardComponent;
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



private:
};

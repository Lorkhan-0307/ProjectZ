// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "Data/Card.h"
#include "InventoryWidget.generated.h"

class UWidgetSwitcher;
class UScrollBox;
class UCraftWidget;
class UBackpackWidget;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UInventoryWidget : public UZUserWidget
{
	GENERATED_BODY()

public:
	virtual void WidgetControllerSet() override;

	void DisplayCardWidget(FCard DisplayCard);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBackpackWidget> BackpackWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCraftWidget> CraftWidgetClass;

	UPROPERTY()
	UBackpackWidget* BackpackWidget;

	UPROPERTY()
	UCraftWidget* CraftWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidgetSwitcher* InventorySwitcher;

private:
	UFUNCTION()
	void UpdateCardInventory();
};

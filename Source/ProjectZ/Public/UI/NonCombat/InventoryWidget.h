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

	UPROPERTY()
	UBackpackWidget* BackpackWidget;

	UPROPERTY()
	UCraftWidget* CraftWidget;

	void DisplayCardWidget(FCard DisplayCard);

	UFUNCTION(BlueprintCallable)
	void UpdateCardInventory();

	UFUNCTION()
	void VisibilityChanged(ESlateVisibility InVisibility);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBackpackWidget> BackpackWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCraftWidget> CraftWidgetClass;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UWidgetSwitcher* InventorySwitcher;

private:
};

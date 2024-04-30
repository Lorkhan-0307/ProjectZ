// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ZWidgetController.h"
#include "InventoryWidgetController.generated.h"


class UCardComponent;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTZ_API UInventoryWidgetController : public UZWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY()
	UCardComponent* CardComponent;

private:

	UFUNCTION()
	void UpdateCardInventory();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZUserWidget.generated.h"

class UCardComponent;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UZUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	FORCEINLINE void SetCardComponent(UCardComponent* CC) { CardComponent = CC; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

	UPROPERTY()
	UCardComponent* CardComponent;

private:
};

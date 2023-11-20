// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardWidget.h"
#include "Blueprint/UserWidget.h"
#include "Data/Card.h"
#include "CardHandWidget.generated.h"

class UOverlay;
class UCardComponent;

UCLASS()
class PROJECTZ_API UCardHandWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UOverlay* CardOverlay;
	
	UPROPERTY(EditAnywhere, Category = "Card")
	TSubclassOf<UCardWidget> CardWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Card")
	TArray<UCardWidget*> HandCard;

	UPROPERTY(BlueprintReadOnly, Category = "Card")
	FCard LeftHandCard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Card")
	FCard RightHandCard;

	
	UFUNCTION(BlueprintCallable)
	void AddCardToHand(FCard NewCard);

private:
	UPROPERTY()
	UCardComponent* CardComponent;
	
};

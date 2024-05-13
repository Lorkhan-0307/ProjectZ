// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Card.h"
#include "BackpackWidget.generated.h"

class UCardDisplayWidget;
class UCardComponent;
struct FCard;
class UCardWidget;
class USizeBox;
class UScrollBox;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UBackpackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	UCardComponent* CardComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCardWidget> CardWidgetClass;

	//UPROPERTY(meta = (BindWidget))
	//USizeBox* BackpackSizeBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* CardScrollBox;

	UPROPERTY(EditAnywhere)
	FVector2D CardSize;

	UPROPERTY()
	UCardDisplayWidget* CardDisplayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCardDisplayWidget> CardDisplayWidgetClass;

	void MakeBackpack();

	UFUNCTION()
	void DisplayCardWidget(FCard ClickCard);

	void DestroyDisplayCardWidget();

	UPROPERTY(EditAnywhere)
	FVector2D WidgetSize = FVector2D(0.f, 0.f);

protected:

private:
	int32 CardRowAmount;
};

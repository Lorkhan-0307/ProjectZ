// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "Data/Card.h"
#include "CardWidget.generated.h"

/**
 * 
 */

class UCanvasPanelSlot;
class UTextBlock;
class UImage;
class UDataTable;
class UCardComponent;
class UCardHandWidget;

UCLASS()
class PROJECTZ_API UCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* CardImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CostText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AtkText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DefText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* CardOutlineImage;

	UPROPERTY()
	UCardComponent* CardComponent;

	virtual void InitCardStatus(FCard CardStatus, bool bSetDelegate = true);

	void DestroyWidget();

	FORCEINLINE FCard GetCardStat() const { return CardStat; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Card)
	UDataTable* CardDataTable;
	
	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D CardSize = FVector2D(210.f, 340.f);

	FCard CardStat;

	UPROPERTY(EditAnywhere, Category = Card)
	TSubclassOf<UCardWidget> CardWidgetClass;

private:
	
};

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

class UTextBlock;
class UImage;
class UDataTable;

UCLASS()
class PROJECTZ_API UCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	//void InitialCard(FCard CardStatus);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* CardImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CardDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ManaText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AtkText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DefText;

	void InitCardStatus(FCard CardStatus, int32 Index);

	void SetCardIndex(int32 Index) { CardIndex = Index; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* CardDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InterpSpeed;

	void SetPosition(float DeltaTime);

private:
	bool bIsInPosition = false;

	int32 CardIndex = 0;

	FWidgetTransform DestinationTransform;
	
};



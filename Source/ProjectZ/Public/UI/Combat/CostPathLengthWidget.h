// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CostPathLengthWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UCostPathLengthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CostText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PathLengthText;
	
	void SetCost(int32 Cost);
	void SetPathLength(float Length);

private:
	int32 RequiredCost;
	float PathLength;
};

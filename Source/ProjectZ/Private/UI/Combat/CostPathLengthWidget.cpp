// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/CostPathLengthWidget.h"

#include "Components/TextBlock.h"

void UCostPathLengthWidget::SetCost(int32 Cost)
{
	RequiredCost = Cost;
	CostText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RequiredCost)));
}

void UCostPathLengthWidget::SetPathLength(float Length)
{
	PathLength = Length;
	PathLengthText->SetText(FText::FromString(FString::Printf(TEXT("%.1f m"), PathLength)));
}

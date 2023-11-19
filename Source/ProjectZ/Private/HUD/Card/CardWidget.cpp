// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Card.h"
#include "Engine/DataTable.h"


void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//InitCardStatus();
}

void UCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bIsInPosition) SetPosition(InDeltaTime);
}

void UCardWidget::InitCardStatus(FCard CardStatus, int32 Index)
{
	CardName->SetText(CardStatus.CardName);
	CardImage->SetBrushFromTexture(CardStatus.CardImage);
	CardDescription->SetText(CardStatus.CardDescription);
	ManaText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
	AtkText->SetText(FText::FromString(FString::FromInt(CardStatus.CardAtk)));
	DefText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
	CardIndex = Index;
	DestinationTransform.Translation.X = CardIndex * 100;
	DestinationTransform.Translation.Y = 0;
}

void UCardWidget::SetPosition(float DeltaTime)
{
	if (GetRenderTransform() == DestinationTransform) return;
	SetRenderTranslation(FMath::Vector2DInterpTo(GetRenderTransform().Translation, DestinationTransform.Translation, DeltaTime, InterpSpeed));
	//FMath::RInterpTo(GetRenderTransform().Angle,)
}

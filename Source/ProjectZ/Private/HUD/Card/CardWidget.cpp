// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardWidget.h"

#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Card.h"
#include "Engine/DataTable.h"
#include "Player/ZPlayerController.h"


void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//InitCardStatus();

	//if (GEngine) GEngine->GameViewport->GetViewportSize(ViewportSize);
	CardSize = GetCachedGeometry().GetLocalSize();

	CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();

	//CardComponent->UpdateHandCardDelegate.AddDynamic(this, &UCardWidget::UpdateCardDestination);
}

void UCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (GetCachedGeometry().GetAbsolutePosition().X != DestinationTransform.Translation.X) SetPosition(InDeltaTime);
}

void UCardWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	DestinationTransform.Translation.Y -= 100;
}

void UCardWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	DestinationTransform.Translation.Y -=100;
}

void UCardWidget::InitCardStatus(FCard CardStatus, int32 Index, FWidgetTransform WidgetTransform)
{
	CardName->SetText(CardStatus.CardName);
	CardImage->SetBrushFromTexture(CardStatus.CardImage);
	CardDescription->SetText(CardStatus.CardDescription);
	ManaText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
	AtkText->SetText(FText::FromString(FString::FromInt(CardStatus.CardAtk)));
	DefText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
	CardIndex = Index;
	SetRenderTranslation(WidgetTransform.Translation);
	DestinationTransform = WidgetTransform;
	//UpdateCardDestination(CardIndex);
}

void UCardWidget::SetPosition(float DeltaTime)
{
	if (GetRenderTransform() == DestinationTransform) return;
	SetRenderTranslation(FMath::Vector2DInterpTo(GetRenderTransform().Translation, DestinationTransform.Translation, DeltaTime, InterpSpeed));
	//FMath::RInterpTo(GetRenderTransform().Angle,)
}

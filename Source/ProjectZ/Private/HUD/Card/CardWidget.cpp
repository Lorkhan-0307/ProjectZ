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

	if (GEngine)
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	CardSize = GetCachedGeometry().GetLocalSize();

	CardComponent = Cast<AZCharacter>(GetOwningPlayer()->GetCharacter())->GetCardComponent();

	CardComponent->UpdateHandCardDelegate.AddDynamic(this, &UCardWidget::UpdateCardDestination);

	SetRenderTranslation(FVector2D(1920,ViewportSize.Y));
	//DestinationTransform.Translation.X = ViewportSize.X;
	DestinationTransform.Translation.Y = ViewportSize.Y;
}

void UCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (GetCachedGeometry().GetAbsolutePosition().X != DestinationTransform.Translation.X) SetPosition(InDeltaTime);
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
	UpdateCardDestination(CardIndex);
}

void UCardWidget::SetPosition(float DeltaTime)
{
	if (GetRenderTransform() == DestinationTransform) return;
	SetRenderTranslation(FMath::Vector2DInterpTo(GetRenderTransform().Translation, DestinationTransform.Translation, DeltaTime, InterpSpeed));
	//FMath::RInterpTo(GetRenderTransform().Angle,)
}

void UCardWidget::UpdateCardDestination(int32 NewHandSize)
{
	int32 NewXPos;
	UE_LOG(LogTemp,Warning,TEXT("%f"),ViewportSize.X);
	if (NewHandSize % 2 == 0)
	{
		NewXPos = /*ViewportSize.X*/960  + (CardIndex - NewHandSize / 2 - 1) * CardDistance - CardSize.X / 2;
	}
	else
	{
		NewXPos = /*ViewportSize.X*/960  + ((float)(CardIndex - NewHandSize / 2 - 1) - 0.5f) * CardDistance - CardSize.X / 2;
	}
	DestinationTransform.Translation.X = NewXPos;
}

void UCardWidget::GetCardPositionCenterX(FVector2D& Pos)
{
	Pos.X = GetCachedGeometry().GetAbsolutePosition().X + CardSize.X / 2;
}

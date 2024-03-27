// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/CharacterPortraitWidget.h"

#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/ZCharacterBase.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Game/ZGameModeBase.h"

void UCharacterPortraitWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetLocation(InDeltaTime);
}

void UCharacterPortraitWidget::InitPortrait(AActor* Actor)
{
	UCharacterClassInfo* CharacterClassInfo = UZAbilitySystemLibrary::GetCharacterClassInfo(this);
	CharacterClass = Cast<AZCharacterBase>(Actor)->GetCharacterClass();
	CharacterPortraitImage->SetBrushFromTexture(CharacterClassInfo->GetClassDefaultInfo(CharacterClass).CharacterPortrait);
	CharacterActor = Actor;

	// For Test
	if (CharacterClass == ECharacterClass::JohnDoe) TestText->SetText(FText::FromString(FString::Printf(TEXT("Player"))));
	else TestText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Actor->GetActorNameOrLabel())));

	Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->TurnChangedDelegate.AddDynamic(this, &UCharacterPortraitWidget::TurnChanged);
}

void UCharacterPortraitWidget::DestroyWidget()
{
	RemoveFromParent();
	CollectGarbage(EObjectFlags::RF_BeginDestroyed);
}

void UCharacterPortraitWidget::SetLocation(float DeltaTime)
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (CanvasPanelSlot == nullptr) return;

	CanvasPanelSlot->SetPosition(FMath::Vector2DInterpTo(CanvasPanelSlot->GetPosition(), DestinationLocation, DeltaTime, InterpSpeed));
}

void UCharacterPortraitWidget::TurnChanged(ETurn Turn)
{
	if (Cast<AZCharacterBase>(CharacterActor)->bIsMyTurn)
	{
		BackGroundImage->SetBrushTintColor(FSlateColor(FColor::Red));
		//BackGroundImage->SetColorAndOpacity(FLinearColor(1,0,0));
	}
	else
	{
		BackGroundImage->SetBrushTintColor(FSlateColor(FColor::Black));
		//BackGroundImage->SetColorAndOpacity(FLinearColor(0,0,0));
	}
}

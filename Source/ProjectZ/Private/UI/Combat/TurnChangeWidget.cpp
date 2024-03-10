// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/TurnChangeWidget.h"

#include "Components/TextBlock.h"
#include "Game/ZGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UTurnChangeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());
}

void UTurnChangeWidget::SetText()
{
	GameMode = GameMode == nullptr ? Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode()) : GameMode;
	CurrentTurn = GameMode->GetCurrentTurn();
	LastTurn = GameMode->GetLastTurn();
	if (CurrentTurn == ETurn::ET_MoveTurn && LastTurn == ETurn::ET_NonCombat)
	{
		TurnChangeText->SetText(FText::FromString("Combat Start"));
	}
	else if (CurrentTurn == ETurn::ET_MoveTurn)
	{
		TurnChangeText->SetText(FText::FromString("Player Turn"));
	}
	else if (CurrentTurn == ETurn::ET_EnemyTurn && (LastTurn == ETurn::ET_MoveTurn || LastTurn == ETurn::ET_PlayerTurn))
	{
		TurnChangeText->SetText(FText::FromString("Enemy Turn"));
	}
	else
	{
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(TurnTextTimer, this, &UTurnChangeWidget::HideText, TurnTextDisplayTime, false);
	GameMode->bCanMove = false;
}

void UTurnChangeWidget::HideText()
{
	SetVisibility(ESlateVisibility::Hidden);
	GameMode->bCanMove = true;
	if (LastTurn == ETurn::ET_NonCombat)
	{
		CharacterPortraitDelegate.Broadcast();
	}
	if (CurrentTurn == ETurn::ET_MoveTurn && LastTurn == ETurn::ET_NonCombat)
	{
		GameMode->SetLastTurn(ETurn::ET_MoveTurn);
		SetText();
		GetWorld()->GetTimerManager().SetTimer(TurnTextTimer, this, &UTurnChangeWidget::HideText, TurnTextDisplayTime, false);
		GameMode->bCanMove = false;
	}
}

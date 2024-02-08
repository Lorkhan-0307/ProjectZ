// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZGameModeBase.h"

#include "AbilitySystem/ZAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ZPlayerState.h"

void AZGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AZGameModeBase::SetTurn(ETurn Turn)
{
	CurrentTurn = Turn;
	TurnChangedDelegate.Broadcast(CurrentTurn);
	if (Turn != ETurn::ET_MoveTurn) return;

	AZPlayerState* PS = Cast<AZPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	UZAttributeSet* AS = Cast<UZAttributeSet>(PS->GetAttributeSet());
	if (AS)
	{
		AS->SetCost(AS->GetMaxCost());
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZGameModeBase.h"

#include "ZGameplayTag.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "AI/ZAIController.h"
#include "Character/ZCharacterBase.h"
#include "Character/ZEnemy.h"
#include "Character/ZPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ZPlayerState.h"
#include "Interaction/CombatInterface.h"

void AZGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AZGameModeBase::CombatStart()
{
	if (CurrentTurn != ETurn::ET_NonCombat) return;

	TArray<AActor*> CombatEnemy;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Enemy"), CombatEnemy);
	UGameplayStatics::GetAllActorsWithTag(this, FName("Player"), CombatActor);

	for (AActor* Actor : CombatEnemy)
	{
		CombatActor.Add(Actor);
	}

	SortCombatActor();
	CombatStartDelegate.Broadcast(CombatActor);

	TurnPlayerIndex = -1;
	TurnEnd();
}

void AZGameModeBase::TurnEnd()
{
	if (CurrentTurn == ETurn::ET_EnemyTurn)
	{
		GetWorld()->GetTimerManager().SetTimer(TurnEndTimer, this, &AZGameModeBase::NextTurn, TurnEndTime, false);
	}
	else
	{
		NextTurn();
	}
}

void AZGameModeBase::CharacterDie(AActor* DieActor)
{
	CombatActor.Remove(DieActor);
	CombatActorChangedDelegate.Broadcast(DieActor);
	if (CombatActor.Num() == 1)
	{
		SetTurn(ETurn::ET_NonCombat);
	}
}

void AZGameModeBase::SetTurn(ETurn Turn)
{
	LastTurn = CurrentTurn;
	CurrentTurn = Turn;
	TurnChangedDelegate.Broadcast(CurrentTurn);
}

void AZGameModeBase::SortCombatActor()
{
	TArray<AZCharacterBase*> SortArray;
	for (AActor* Actor : CombatActor)
	{
		SortArray.Add(Cast<AZCharacterBase>(Actor));
	}

	SortArray.Sort([](const AZCharacterBase& A, const AZCharacterBase& B)
	{
		return A.GetCombatPriority() > B.GetCombatPriority();
	});

	CombatActor.Empty();
	for (AZCharacterBase* Character : SortArray)
	{
		CombatActor.Add(Cast<AActor>(Character));
	}

	for (int i = 0; i < CombatActor.Num() - 1; i++)
	{
		if (Cast<AZCharacterBase>(CombatActor[i])->GetCombatPriority() == Cast<AZCharacterBase>(CombatActor[i + 1])->GetCombatPriority())
		{
			if (FMath::RandRange(0, 1)) CombatActor.Swap(i, i + 1);
		}
	}
}

void AZGameModeBase::NextTurn()
{
	TurnPlayerIndex++;
	if (TurnPlayerIndex >= CombatActor.Num())
	{
		TurnPlayerIndex = 0;
	}

	TurnActor = CombatActor[TurnPlayerIndex];
	for (FDebuff& Debuff : Cast<ICombatInterface>(TurnActor)->Debuffs)
	{
		if (Debuff.DebuffType.MatchesTagExact(FZGameplayTag::Get().Debuff_Stun) && Debuff.DebuffDuration > 0)
		{
			Debuff.DebuffDuration--;
		}
	}

	if (AZPlayerCharacter* PlayerCharacter = Cast<AZPlayerCharacter>(TurnActor))
	{
		SetTurn(ETurn::ET_MoveTurn);
	}
	else if (AZEnemy* Enemy = Cast<AZEnemy>(TurnActor))
	{
		SetTurn(ETurn::ET_EnemyTurn);
		Enemy->bIsMyTurn = true;
	}
}

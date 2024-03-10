// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "Character/ZEnemy.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZEnemy* Character = Cast<AZEnemy>(AIOwner->GetCharacter());
	UZAbilitySystemLibrary::PayCost(Character, Cast<UZAttributeSet>(Character->GetAttributeSet())->GetCost());
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

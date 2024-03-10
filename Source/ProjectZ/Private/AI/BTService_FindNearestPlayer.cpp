// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "AI/ZAIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Character/ZEnemy.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorWithTag);

	LastLocation = CurrentLocation;
	CurrentLocation = OwningPawn->GetActorLocation();
	const float DeltaLocation = FVector::DistXY(LastLocation, CurrentLocation) / 100.f;

	AZCharacterBase* Character = Cast<AZCharacterBase>(OwningPawn);
	const UZAttributeSet* AS = Cast<UZAttributeSet>(Character->GetAttributeSet());
	UZAbilitySystemLibrary::PayCost(Character, DeltaLocation);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, CostSelector, FMath::Floor(AS->GetCost()));

	if (FMath::Floor(AS->GetCost()) == 0 && Cast<AZEnemy>(Character)->bIsMyTurn)
	{
		Cast<AZEnemy>(Character)->bIsMyTurn = false;
		Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->NextTurn();
	}


	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : ActorWithTag)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}

	/*
	AZAIController* AIController = Cast<AZAIController>(AIOwner);
	if (AIController && AIController->TargetActor)
	{
		UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, AIController->TargetActor);
		UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, OwningPawn->GetDistanceTo(AIController->TargetActor));
	}
	*/

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);

	UBTFunctionLibrary::SetBlackboardValueAsBool(this, MyTurnSelector, Cast<AZEnemy>(Character)->bIsMyTurn);
	UBTFunctionLibrary::SetBlackboardValueAsBool(this, CanMoveSelector, Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->bCanMove);
}

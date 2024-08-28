// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ZAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ZCharacterBase.h"
#include "Character/ZEnemy.h"
#include "Character/ZPlayerCharacter.h"
#include "Game/ZGameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AZAIController::AZAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AZAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AZAIController::FindTarget);
}

void AZAIController::FindTarget(AActor* Actor, FAIStimulus AIStimulus)
{
	if (ICombatInterface::Execute_IsDead(GetPawn())) return;
	if (Cast<AZPlayerCharacter>(Actor) == nullptr) return;
	TargetActor = Actor;
	AZGameModeBase* GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode && GameMode->GetCurrentTurn() == ETurn::ET_NonCombat)
	{
		GameMode->CombatStart(Cast<AZEnemy>(GetPawn())->GetRoomNo());
	}
}

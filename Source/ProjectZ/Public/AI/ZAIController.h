// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZAIController.generated.h"

struct FAIStimulus;
class UBehaviorTreeComponent;
class UAISense;

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZAIController();

	UPROPERTY()
	AActor* TargetActor;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

private:
	UFUNCTION()
	void FindTarget(AActor* Actor, FAIStimulus AIStimulus);
};

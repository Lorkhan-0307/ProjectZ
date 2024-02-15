// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ZGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly, Category = Card)
	FGameplayTag CardSkillTag;
};

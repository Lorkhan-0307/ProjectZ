// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterBase.h"
#include "ZPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZPlayerCharacter : public AZCharacterBase
{
	GENERATED_BODY()
public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;

	virtual int32 GetLevel() override;
	
};

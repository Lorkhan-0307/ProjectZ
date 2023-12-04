// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterBase.h"
#include "ZCharacter.generated.h"

class UCameraComponent;
class UCardComponent;

UCLASS()
class PROJECTZ_API AZCharacter : public AZCharacterBase
{
	GENERATED_BODY()

public:
	AZCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;

	virtual int32 GetLevel() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FirstPersonCamera;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterBase.h"
#include "ZCombatCharacter.generated.h"

class UCameraComponent;
class UCardComponent;
class USpringArmComponent;

UCLASS()
class PROJECTZ_API AZCombatCharacter : public AZCharacterBase
{
	GENERATED_BODY()

public:
	AZCombatCharacter();

private:
	UPROPERTY(EditAnywhere, Category = Camera) USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent* TCamera;
};

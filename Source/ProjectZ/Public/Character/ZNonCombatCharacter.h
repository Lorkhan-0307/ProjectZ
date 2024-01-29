// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZPlayerCharacter.h"
#include "ZNonCombatCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCardComponent;

UCLASS()
class PROJECTZ_API AZNonCombatCharacter : public AZPlayerCharacter
{
	GENERATED_BODY()

public:
	AZNonCombatCharacter();

	UPROPERTY(EditAnywhere, Category = Camera)
	UCameraComponent* FirstPersonCamera;

private:
	UPROPERTY(EditAnywhere, Category = Camera)
	USpringArmComponent* SpringArmComponent;
};

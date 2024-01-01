// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZPlayerCharacter.h"
#include "ZNonCombatCharacter.generated.h"

class UCameraComponent;
class UCardComponent;

UCLASS()
class PROJECTZ_API AZNonCombatCharacter : public AZPlayerCharacter
{
	GENERATED_BODY()

public:
	AZNonCombatCharacter();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FirstPersonCamera;
};

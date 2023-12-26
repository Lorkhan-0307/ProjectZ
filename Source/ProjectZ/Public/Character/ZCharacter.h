// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZPlayerCharacter.h"
#include "ZCharacter.generated.h"

class UCameraComponent;
class UCardComponent;

UCLASS()
class PROJECTZ_API AZCharacter : public AZPlayerCharacter
{
	GENERATED_BODY()

public:
	AZCharacter();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FirstPersonCamera;
};

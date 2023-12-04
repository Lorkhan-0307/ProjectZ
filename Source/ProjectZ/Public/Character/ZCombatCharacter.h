// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterBase.h"
#include "ZCombatCharacter.generated.h"

class UCameraComponent;
class UCardComponent;

UCLASS()
class PROJECTZ_API AZCombatCharacter : public AZCharacterBase
{
	GENERATED_BODY()
	
public:
	AZCombatCharacter();

	UPROPERTY(EditAnywhere)
	UCardComponent* CardComponent;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCardComponent* GetCardComponent() const { return CardComponent; }

private:
	UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent* Camera;
};

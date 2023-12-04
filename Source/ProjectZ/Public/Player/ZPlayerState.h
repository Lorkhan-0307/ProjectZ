// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ZPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class PROJECTZ_API AZPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AZPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE void SetCharacterName(FName Name) { CharacterName = Name; }
	FORCEINLINE FName GetCharacterName() const { return CharacterName; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	// Not use for our game (maybe)
	UPROPERTY(VisibleAnywhere)
	int32 Level = 1.f;

	FName CharacterName;
};

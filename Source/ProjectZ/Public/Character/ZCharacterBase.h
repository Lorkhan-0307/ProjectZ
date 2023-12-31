// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "ZCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS(Abstract)
class PROJECTZ_API AZCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AZCharacterBase();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCardComponent* CardComponent;

	// Move function to ZAbilitySystemLibrary
	//void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCardComponent* GetCardComponent() const { return CardComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;

protected:
	virtual void BeginPlay() override;
	virtual int32 GetLevel();

	virtual FVector GetCombatSocketLocation() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "Character Class Default")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "Character Class Default")
	ECharacterClass CharacterClass = ECharacterClass::JohnDoe;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	virtual void InitAbilityActorInfo();

	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbility();

private:
	UPROPERTY(EditAnywhere, Category = Ability)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbility;

	TObjectPtr<UAnimMontage> HitReactMontage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
class UAnimMontage;
struct FGameplayEffectSpec;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
};

USTRUCT(BlueprintType)
struct FDebuff
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag DebuffType;

	UPROPERTY()
	float DebuffDamage;

	UPROPERTY()
	int32 DebuffDuration;

	UPROPERTY()
	int32 DebuffStack;

	FGameplayEffectSpec* DebuffEffectSpec;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTZ_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
	
	TArray<FDebuff> Debuffs;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddDebuff(FDebuff Debuff);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveDebuff(FGameplayTag RemoveDebuffType);

	virtual FOnASCRegistered GetOnASCRegisterdDelegate() = 0;
	virtual FOnDeath GetOnDeathDelegate() = 0;
};

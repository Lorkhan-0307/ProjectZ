// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZAbilitySystemLibrary.generated.h"

class AZCharacterBase;
struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
class UOverlayWidgetController;
class UAbilitySystemComponent;

// Get OverlayWidgetController in Blueprint
UCLASS()
class PROJECTZ_API UZAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "zAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static bool IsDodged(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetIsDodged(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodged);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable)
	static bool PayCost(AZCharacterBase* Character, float Cost);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZAbilityType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
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
	static void GiveStartupAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static bool IsDodged(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static bool IsBlocked(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static int32 GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDebuffType(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static int32 GetDebuffStack(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetBuffType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetBuffAttribute(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static float GetBuffMagnitude(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static int32 GetBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetIsDodged(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodged);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlocked(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlocked);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, int32 InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDebuffType);
	
	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffStack(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, int32 InDebuffStack);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetBuffType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InBuffType);
	
	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetBuffAttribute(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InBuffAttribute);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetBuffMagnitude(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InBuffMagnitude);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayEffects")
	static void SetBuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, int32 InBuffDuration);

	UFUNCTION(BlueprintCallable)
	static bool PayCost(AZCharacterBase* Character, float Cost);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|GameplayMechanics")
	static void GetSectorFormTarget(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Angle, float Radius, const FVector& SphereOrigin, const FVector& MousePos);

	UFUNCTION(BlueprintCallable, Category = "ZAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};

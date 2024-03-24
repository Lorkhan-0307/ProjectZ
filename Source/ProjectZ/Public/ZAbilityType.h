#pragma once

#include "GameplayEffectTypes.h"
#include "ZAbilityType.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
	{
	}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	TArray<FGameplayTag> DebuffTypes;

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	int32 DebuffDuration = 0;

	UPROPERTY()
	int32 DebuffStack = 0;
};

// Custom GameplayEffectContext
USTRUCT(BlueprintType)
struct FZGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsDodged() const { return bIsDodged; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	int32 GetDebuffDuration() const { return DebuffDuration; }
	TArray<FGameplayTag> GetDebuffTypes() const { return DebuffTypes; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	int32 GetDebuffStack() const { return DebuffStack; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsDodged(bool bInIsDodged) { bIsDodged = bInIsDodged; }
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(int32 InDuration) { DebuffDuration = InDuration; }
	void SetDebuffTypes(TArray<FGameplayTag> InDebuffTypes) { DebuffTypes = InDebuffTypes; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDebuffStack(int32 InDebuffStack) { DebuffStack = InDebuffStack; }

	// Returns the actual struct used for serialization, subclasses must override this
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FZGameplayEffectContext::StaticStruct();
	}

	// Creates a copy of this context, used to duplicate for later modifications
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result;
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// Custom serialization, subclasses must override this
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsDodged = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	int32 DebuffDuration = 0;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	TArray<FGameplayTag> DebuffTypes;

	UPROPERTY()
	int32 DebuffStack = 0;
};

template <>
struct TStructOpsTypeTraits<FZGameplayEffectContext> : TStructOpsTypeTraitsBase2<FZGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

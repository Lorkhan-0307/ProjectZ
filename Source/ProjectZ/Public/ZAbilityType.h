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
	FGameplayTag DebuffType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	int32 DebuffDuration = 0;

	UPROPERTY()
	int32 DebuffStack = 0;

	UPROPERTY()
	FGameplayTag BuffType = FGameplayTag();

	UPROPERTY()
	FGameplayTag BuffAttribute;

	UPROPERTY()
	float BuffMagnitude;

	UPROPERTY()
	int32 BuffDuration;
};

// Custom GameplayEffectContext
USTRUCT(BlueprintType)
struct FZGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsDodged() const { return bIsDodged; }
	bool IsBlocked() const { return bIsBlocked; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	int32 GetDebuffDuration() const { return DebuffDuration; }
	FGameplayTag GetDebuffType() const { return DebuffType; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	int32 GetDebuffStack() const { return DebuffStack; }
	FGameplayTag GetBuffType() const { return BuffType; }
	FGameplayTag GetBuffAttribute() const { return BuffAttribute; }
	float GetBuffMagnitude() const { return BuffMagnitude; }
	int32 GetBuffDuration() const { return BuffDuration; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsDodged(bool bInIsDodged) { bIsDodged = bInIsDodged; }
	void SetIsBlocked(bool bInIsBlocked) { bIsBlocked = bInIsBlocked; }
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(int32 InDuration) { DebuffDuration = InDuration; }
	void SetDebuffType(FGameplayTag InDebuffType) { DebuffType = InDebuffType; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDebuffStack(int32 InDebuffStack) { DebuffStack = InDebuffStack; }
	void SetBuffType(FGameplayTag InBuffType) { BuffType = InBuffType; }
	void SetBuffAttribute(FGameplayTag InBuffAttribute) { BuffAttribute = InBuffAttribute; }
	void SetBuffMagnitude(float InBuffMagnitude) { BuffMagnitude = InBuffMagnitude; }
	void SetBuffDuration(int32 InBuffDuration) { BuffDuration = InBuffDuration; }

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
	bool bIsBlocked = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	int32 DebuffDuration = 0;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FGameplayTag DebuffType;

	UPROPERTY()
	int32 DebuffStack = 0;

	UPROPERTY()
	FGameplayTag BuffType;

	UPROPERTY()
	FGameplayTag BuffAttribute;

	UPROPERTY()
	float BuffMagnitude = 0.f;

	UPROPERTY()
	int32 BuffDuration = 0;
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

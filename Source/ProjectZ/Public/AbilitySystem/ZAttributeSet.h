// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ZAttributeSet.generated.h"


// Macro For Attributes Getter, Setter
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

template<class T>
using TStaticFunPtr = typename TBaseStaticDelegateInstance<T,FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class PROJECTZ_API UZAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UZAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag,TStaticFunPtr<FGameplayAttribute()>> TagToAttribute;

	// Vital Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Health) // Make Getter, Setter

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Mentality;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Mentality)

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Cost;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Cost)

	UPROPERTY(BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxCost;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, MaxCost)

	

	// Primary Attributes : For Passive Cards
	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Dexterity)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Sociability;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Sociability)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData AntiSociability;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, AntiSociability)

	UPROPERTY(BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Engineering;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Engineering)

	// Secondary Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMentality;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, MaxMentality)

	// TO DO : Set Secondary Attribute calculation in GameplayEffect that setting Secondary Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, ArmorPenetration)
	
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData DodgeChance;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, DodgeChance)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, CriticalHitChance)

	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, CriticalHitDamage)
	
	UPROPERTY(BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, CriticalHitResistance)

	// Resistance Attributes

	UPROPERTY(BlueprintReadOnly, Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, PhysicalResistance)

	// Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UZAttributeSet, IncomingDamage)

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};

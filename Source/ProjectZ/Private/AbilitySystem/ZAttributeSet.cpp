// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

UZAttributeSet::UZAttributeSet()
{
	const FZGameplayTag& GameplayTag = FZGameplayTag::Get();

	TagToAttribute.Add(GameplayTag.Attributes_Primary_Sociability, GetSociabilityAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Primary_AntiSociability, GetAntiSociabilityAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Primary_Strength, GetStrengthAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Primary_Dexterity, GetDexterityAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Primary_Engineering, GetEngineeringAttribute);

	TagToAttribute.Add(GameplayTag.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Secondary_MaxMentality, GetMaxMentalityAttribute);
}

// Clamp Attributes
void UZAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// Not clamping actual value. just NewValue.
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetMentalityAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMentality());
	}
}

// Called just before a GameplayEffect is executed to modify the base value of an attribute
void UZAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// Clamping real Attributes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetMentalityAttribute())
	{
		SetMentality(FMath::Clamp(GetMentality(), 0.f, GetMaxMentality()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FZGameplayTag::Get().Effect_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bDodge = UZAbilitySystemLibrary::IsDodged(Props.EffectContextHandle);
			const bool bCriticalHit = UZAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			if (bDodge)
			{
				UE_LOG(LogTemp, Warning, TEXT("Dodge"));
			}
			if (bCriticalHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("Critical"));
			}
		}
	}
}

void UZAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source : causer of the effect
	// Target : target of the effect (owner of this)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

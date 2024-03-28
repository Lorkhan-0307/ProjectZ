// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ZPlayerControllerBase.h"

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
	TagToAttribute.Add(GameplayTag.Attributes_Secondary_Armor, GetArmorAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Secondary_DodgeChance, GetDodgeChanceAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);

	TagToAttribute.Add(GameplayTag.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);

	TagToAttribute.Add(GameplayTag.Attributes_Card_WeaponAtk, GetWeaponAtkAttribute);
	TagToAttribute.Add(GameplayTag.Attributes_Card_Defence, GetDefenceAttribute);
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

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter))
	{
		return;
	}

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
		HandleIncomingDamage(Props);
	}
}

void UZAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
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

	SetDefence(0.f);

	const bool bDodge = UZAbilitySystemLibrary::IsDodged(Props.EffectContextHandle);
	const bool bCriticalHit = UZAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);

	ShowFloatingText(Props, LocalIncomingDamage);

	if (UZAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
	{
		Debuff(Props);
	}
}

void UZAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FZGameplayTag& GameplayTag = FZGameplayTag::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UZAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UZAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const int32 DebuffDuration = UZAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const FGameplayTag DebuffType = UZAbilitySystemLibrary::GetDebuffType(Props.EffectContextHandle);
	const int32 DebuffStack = UZAbilitySystemLibrary::GetDebuffStack(Props.EffectContextHandle);

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());

	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	Effect->InheritableOwnedTagsContainer.AddTag(DebuffType);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	//Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UZAttributeSet::GetIncomingDamageAttribute();

	FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f);
	if (MutableSpec)
	{
		FZGameplayEffectContext* ZContext = static_cast<FZGameplayEffectContext*>(EffectContext.Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		ZContext->SetDamageType(DebuffDamageType);
	}

	FDebuff NewDebuff;
	NewDebuff.DebuffDamage = DebuffDamage;
	NewDebuff.DebuffDuration = DebuffDuration;
	NewDebuff.DebuffStack = DebuffStack;
	NewDebuff.DebuffType = DebuffType;
	NewDebuff.DebuffEffectSpec = MutableSpec;
	ICombatInterface::Execute_AddDebuff(Props.TargetCharacter, NewDebuff);
}

void UZAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (AZPlayerControllerBase* PC = Cast<AZPlayerControllerBase>(UGameplayStatics::GetPlayerController(Props.SourceCharacter, 0)))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter);
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

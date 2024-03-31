// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "Game/ZGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ZPlayerState.h"
#include "Ui/HUD/ZHUDBase.h"
#include "UI/WidgetController/ZWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemComponent.h"
#include "ZAbilityType.h"
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "Character/ZCharacterBase.h"

UOverlayWidgetController* UZAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AZHUDBase* ZHUD = Cast<AZHUDBase>(PC->GetHUD()))
		{
			AZPlayerState* PS = PC->GetPlayerState<AZPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return ZHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UZAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AZGameModeBase* ZGameMode = Cast<AZGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ZGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttribute, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttribute, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttribute, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UZAbilitySystemLibrary::GiveStartupAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	AZGameModeBase* ZGameMode = Cast<AZGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ZGameMode == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbility)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbility)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UZAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AZGameModeBase* ZGameMode = Cast<AZGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ZGameMode == nullptr) return nullptr;
	return ZGameMode->CharacterClassInfo;
}

bool UZAbilitySystemLibrary::IsDodged(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->IsDodged();
	}
	return false;
}

bool UZAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->IsCriticalHit();
	}
	return false;
}

bool UZAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UZAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

int32 UZAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetDebuffDuration();
	}
	return 0;
}

FGameplayTag UZAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (ZEffectContext->GetDamageType().IsValid())
		{
			return *ZEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FGameplayTag UZAbilitySystemLibrary::GetDebuffType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetDebuffType();
	}
	return FGameplayTag();
}

int32 UZAbilitySystemLibrary::GetDebuffStack(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetDebuffStack();
	}
	return 0;
}

FGameplayTag UZAbilitySystemLibrary::GetBuffType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetBuffType();
	}
	return FGameplayTag();
}

FGameplayTag UZAbilitySystemLibrary::GetBuffAttribute(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetBuffAttribute();
	}
	return FGameplayTag();
}

float UZAbilitySystemLibrary::GetBuffMagnitude(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetBuffMagnitude();
	}
	return 0.f;
}

int32 UZAbilitySystemLibrary::GetBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FZGameplayEffectContext* ZEffectContext = static_cast<const FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ZEffectContext->GetBuffDuration();
	}
	return 0;
}

void UZAbilitySystemLibrary::SetIsDodged(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodged)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetIsDodged(bInIsDodged);
	}
}

void UZAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UZAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UZAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UZAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, int32 InDebuffDuration)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UZAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		ZEffectContext->SetDamageType(DamageType);
	}
}

void UZAbilitySystemLibrary::SetDebuffType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDebuffTypes)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetDebuffType(InDebuffTypes);
	}
}

void UZAbilitySystemLibrary::SetDebuffStack(FGameplayEffectContextHandle& EffectContextHandle, int32 InDebuffStack)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetDebuffStack(InDebuffStack);
	}
}

void UZAbilitySystemLibrary::SetBuffType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InBuffType)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetBuffType(InBuffType);
	}
}

void UZAbilitySystemLibrary::SetBuffAttribute(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InBuffAttribute)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetBuffAttribute(InBuffAttribute);
	}
}

void UZAbilitySystemLibrary::SetBuffMagnitude(FGameplayEffectContextHandle& EffectContextHandle, float InBuffMagnitude)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetBuffMagnitude(InBuffMagnitude);
	}
}

void UZAbilitySystemLibrary::SetBuffDuration(FGameplayEffectContextHandle& EffectContextHandle, int32 InBuffDuration)
{
	if (FZGameplayEffectContext* ZEffectContext = static_cast<FZGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ZEffectContext->SetBuffDuration(InBuffDuration);
	}
}

bool UZAbilitySystemLibrary::PayCost(AZCharacterBase* Character, float Cost)
{
	UZAttributeSet* AS = Cast<UZAttributeSet>(Character->GetAttributeSet());
	if (AS && AS->GetCost() >= Cost)
	{
		AS->SetCost(AS->GetCost() - Cost);
		return true;
	}
	else
	{
		return false;
	}
}

void UZAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

FGameplayEffectContextHandle UZAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FZGameplayTag& GameplayTag = FZGameplayTag::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DebuffType, 1);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Duration, DamageEffectParams.DebuffDuration);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.BuffType, 1);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.BuffAttribute, 1);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Buff_Magnitude, DamageEffectParams.BuffMagnitude);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Buff_Duration, DamageEffectParams.BuffDuration);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

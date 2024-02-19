// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZAbilitySystemLibrary.h"

#include "GameplayEffectTypes.h"
#include "Game/ZGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ZPlayerState.h"
#include "Ui/HUD/ZHUDBase.h"
#include "UI/WidgetController/ZWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystemComponent.h"
#include "ZAbilityType.h"
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

void UZAbilitySystemLibrary::GiveStartupAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AZGameModeBase* ZGameMode = Cast<AZGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ZGameMode == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbility)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UZAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AZGameModeBase* AuraGameMode = Cast<AZGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
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

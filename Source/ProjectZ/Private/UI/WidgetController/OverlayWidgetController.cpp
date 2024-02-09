// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "Game/ZGameModeBase.h"

// Set Initial value to overlay
void UOverlayWidgetController::BroadcastInitialValues()
{
	const UZAttributeSet* ZAttributeSet = CastChecked<UZAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(ZAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ZAttributeSet->GetMaxHealth());
	OnMentalityChanged.Broadcast(ZAttributeSet->GetMentality());
	OnMaxMentalityChanged.Broadcast(ZAttributeSet->GetMaxMentality());
	OnCostChanged.Broadcast(ZAttributeSet->GetCost());
	OnMaxCostChanged.Broadcast(ZAttributeSet->GetMaxCost());
}

// Bind delegates
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UZAttributeSet* ZAttributeSet = CastChecked<UZAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetMentalityAttribute()).AddUObject(this, &UOverlayWidgetController::MentalityChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetMaxMentalityAttribute()).AddUObject(this, &UOverlayWidgetController::MaxMentalityChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetCostAttribute()).AddUObject(this, &UOverlayWidgetController::CostChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetMaxCostAttribute()).AddUObject(this, &UOverlayWidgetController::MaxCostChanged);

	Cast<UZAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda([](const FGameplayTagContainer& AssetTags)
	{
		// Not Implemented Yet
	});
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MentalityChanged(const FOnAttributeChangeData& Data) const
{
	OnMentalityChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxMentalityChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxMentalityChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::CostChanged(const FOnAttributeChangeData& Data) const
{
	OnCostChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxCostChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxCostChanged.Broadcast(Data.NewValue);
}


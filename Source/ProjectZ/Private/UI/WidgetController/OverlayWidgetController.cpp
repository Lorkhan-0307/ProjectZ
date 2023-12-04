// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAttributeSet.h"

// Set Initial value to overlay
void UOverlayWidgetController::BroadcastInitialValues()
{
	const UZAttributeSet* ZAttributeSet = CastChecked<UZAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(ZAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ZAttributeSet->GetMaxHealth());
	OnMentalityChanged.Broadcast(ZAttributeSet->GetMentality());
	OnMaxMentalityChanged.Broadcast(ZAttributeSet->GetMaxMentality());
	
}

// Bind delegates
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UZAttributeSet* ZAttributeSet = CastChecked<UZAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetHealthAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetMaxHealthAttribute()).AddUObject(this,&UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetMentalityAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAttributeSet->GetMaxMentalityAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChanged);

	Cast<UZAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda([](const FGameplayTagContainer& AssetTags)
	{
		// Not Implement Yet
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

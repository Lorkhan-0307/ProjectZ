// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZPlayerControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Input/ZInputComponent.h"

void AZPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	ZInputComponent = CastChecked<UZInputComponent>(InputComponent);
	// Bind Input Action with tags
	ZInputComponent->BindAbilityAction(InputConfig, this, &AZPlayerControllerBase::AbilityInputTagPressed, &AZPlayerControllerBase::AbilityInputTagReleased, &AZPlayerControllerBase::AbilityInputTagHeld);
}

void AZPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AZPlayerControllerBase::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AZPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UZAbilitySystemComponent* AZPlayerControllerBase::GetASC()
{
	if (ZAbilitySystemComponent == nullptr)
	{
		ZAbilitySystemComponent = Cast<UZAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return ZAbilitySystemComponent;
}
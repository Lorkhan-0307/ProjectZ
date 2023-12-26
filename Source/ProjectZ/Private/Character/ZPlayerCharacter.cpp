// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "UI/HUD/ZNonCombatHUD.h"
#include "Player/ZNonCombatPlayerController.h"
#include "Player/ZPlayerState.h"


// When PlayerController Possess the Character
void AZPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbility();
}

// Setup Character
void AZPlayerCharacter::InitAbilityActorInfo()
{
	if (AbilitySystemComponent) return;
	AZPlayerState* ZPlayerState = GetPlayerState<AZPlayerState>();
	check(ZPlayerState);
	ZPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ZPlayerState, this);
	Cast<UZAbilitySystemComponent>(ZPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = ZPlayerState->GetAbilitySystemComponent();
	AttributeSet = ZPlayerState->GetAttributeSet();

	if (AZPlayerControllerBase* ZPlayerController = Cast<AZPlayerControllerBase>(GetController()))
	{
		if (AZHUDBase* ZHUD = Cast<AZHUDBase>(ZPlayerController->GetHUD()))
		{
			ZHUD->InitOverlay(ZPlayerController, GetPlayerState(), GetAbilitySystemComponent(), GetAttributeSet(),this);
		}
	}
	InitializeDefaultAttributes();

	// TO DO : Make Difference by game mode
	CardComponent->InitializeNonCombat(this);
	CardComponent->InitializeCombat(this);
}

int32 AZPlayerCharacter::GetLevel()
{
	const AZPlayerState* ZPlayerState = GetPlayerState<AZPlayerState>();
	return ZPlayerState->GetPlayerLevel();
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZNonCombatPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Input/ZInputComponent.h"

void AZNonCombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);

	check(ZContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(ZContext, 0);
	}

	CardComponent = Cast<AZNonCombatCharacter>(GetCharacter())->GetCardComponent();
}

void AZNonCombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ZInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZNonCombatPlayerController::Move);
	ZInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZNonCombatPlayerController::Look);
	
}

void AZNonCombatPlayerController::Move(const FInputActionValue& Value)
{
	/*
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
	}
	*/
}

void AZNonCombatPlayerController::Look(const FInputActionValue& InputActionValue)
{
	/*
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
	*/
}
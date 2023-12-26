// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZNonCombatPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "Input/ZInputComponent.h"

void AZNonCombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(ZContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(ZContext, 0);
	}

	CardComponent = Cast<AZCharacter>(GetCharacter())->GetCardComponent();
}

void AZNonCombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UZInputComponent* ZInputComponent = CastChecked<UZInputComponent>(InputComponent);

	ZInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZNonCombatPlayerController::Move);
	ZInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZNonCombatPlayerController::Look);

	// Bind Input Action with tags
	ZInputComponent->BindAbilityAction(InputConfig, this, &AZNonCombatPlayerController::AbilityInputTagPressed, &AZNonCombatPlayerController::AbilityInputTagReleased, &AZNonCombatPlayerController::AbilityInputTagHeld);
}

void AZNonCombatPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
	}
}

void AZNonCombatPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
}

void AZNonCombatPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AZNonCombatPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AZNonCombatPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UZAbilitySystemComponent* AZNonCombatPlayerController::GetASC()
{
	if (ZAbilitySystemComponent == nullptr)
	{
		ZAbilitySystemComponent = Cast<UZAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return ZAbilitySystemComponent;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZNonCombatPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"

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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZNonCombatPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZNonCombatPlayerController::Look);
	EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &AZNonCombatPlayerController::Test);
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

void AZNonCombatPlayerController::Test()
{
	//if (CardComponent->GetDeckSize()>0)
		//CardComponent->DrawAndAddCardDelegate.Broadcast(CardComponent->DrawCard());
}

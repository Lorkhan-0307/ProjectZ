// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/CardComponent.h"
#include "Character/ZCharacter.h"
#include "HUD/Card/CardHandHUD.h"

void AZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(ZContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(ZContext, 0);
	}

	CardComponent = Cast<AZCharacter>(GetCharacter())->GetCardComponent();

	UCardHandHUD* CardHandHUD = CreateWidget<UCardHandHUD>(this,CardHandHUDClass);
	CardHandHUD->AddToViewport();
}

void AZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZPlayerController::Look);
	EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Triggered, this, &AZPlayerController::Test);
}

void AZPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
	}
}

void AZPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
}

void AZPlayerController::Test()
{
	//if (CardComponent->GetDeckSize()>0)
		//CardComponent->DrawAndAddCardDelegate.Broadcast(CardComponent->DrawCard());
}

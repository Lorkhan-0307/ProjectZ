// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZCombatPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AZCombatPlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(ZContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(ZContext, 0);
    }
}

void AZCombatPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZCombatPlayerController::MoveCam);
}

void AZCombatPlayerController::MoveCam(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddControllerPitchInput(MovementVector.Y);
        ControlledPawn->AddControllerYawInput(-MovementVector.X);
    }

}
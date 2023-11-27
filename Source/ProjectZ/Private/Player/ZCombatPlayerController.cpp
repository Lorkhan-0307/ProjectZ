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
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZCombatPlayerController::RotateYaw);
}

void AZCombatPlayerController::MoveCam(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        //ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
        //ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
        ControlledPawn->AddControllerPitchInput(MovementVector.Y);
        ControlledPawn->AddControllerYawInput(MovementVector.X);
    }

}

void AZCombatPlayerController::RotateYaw(const FInputActionValue& Value)
{
    // Handle rotating the camera left and right
}
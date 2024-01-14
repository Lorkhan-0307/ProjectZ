// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ZCombatPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Input/ZInputComponent.h"

void AZCombatPlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(ZContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(ZContext, 0);
    }
    SetControlRotation(FRotator::MakeFromEuler(FVector(0.f, 345.f, 0.f)));
}

void AZCombatPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    ZInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZCombatPlayerController::MoveCam);
    ZInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AZCombatPlayerController::ZoomCam);
}

void AZCombatPlayerController::MoveCam(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        TargetRot = ControlledPawn->GetControlRotation();
        TargetRot = FRotator::MakeFromEuler(FVector(TargetRot.Yaw - MovementVector.X, TargetRot.Pitch - MovementVector.Y, TargetRot.Roll));
        SetControlRotation(FRotator::MakeFromEuler(FVector(TargetRot.Yaw, FMath::Clamp(TargetRot.Pitch, 270.f, 359.9f), TargetRot.Roll)));
    }
}

void AZCombatPlayerController::ZoomCam(const FInputActionValue& Value)
{
    const float MovementFloat = Value.Get<float>();
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        USpringArmComponent* SpringArmComponent = ControlledPawn->FindComponentByClass<USpringArmComponent>();
        if(SpringArmComponent)
        {
            SpringArmComponent->TargetArmLength+=MovementFloat*100;
            SpringArmComponent->TargetArmLength=FMath::Clamp(SpringArmComponent->TargetArmLength, 1000, 3000);
        }
    }
}
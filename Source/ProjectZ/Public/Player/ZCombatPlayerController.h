// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ZCombatPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZCombatPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

    // Called to bind functionality to input
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    // Functions to handle input
    

private:
    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputMappingContext> ZContext;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> LookAction;

    void MoveCam(const FInputActionValue& InputActionValue);
    void RotateYaw(const FInputActionValue& InputActionValue);

    // Camera movement and rotation speed
    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float CameraMoveSpeed = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float CameraRotateSpeed = 50.0f;
};

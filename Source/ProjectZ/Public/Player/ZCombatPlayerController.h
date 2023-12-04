// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ZCombatPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECTZ_API AZCombatPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputMappingContext> ZContext;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> ZoomAction;

    void MoveCam(const FInputActionValue& InputActionValue);
    void ZoomCam(const FInputActionValue& InputActionValue);
    FRotator TargetRot;

    // Camera movement and rotation speed
    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float CameraMoveSpeed = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Camera Settings")
    float CameraRotateSpeed = 50.0f;
};

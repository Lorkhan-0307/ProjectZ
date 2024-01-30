// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "ZPlayerControllerBase.generated.h"

class UZInputConfig;
class UZInputComponent;
class UInputMappingContext;
class UInputAction;
class UCardComponent;
class UZAbilitySystemComponent;
class USplineComponent;
struct FInputActionValue;

UCLASS()
class PROJECTZ_API AZPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AZPlayerControllerBase();
	virtual void SetupInputComponent() override;

protected:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	UCardComponent* CardComponent;

	UPROPERTY()
	UZInputComponent* ZInputComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UZInputConfig> InputConfig;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CameraMoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CameraRotateAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CameraResetAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CameraZoomAction;

	UPROPERTY()
	TObjectPtr<UZAbilitySystemComponent> ZAbilitySystemComponent;

	UZAbilitySystemComponent* GetASC();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void CameraMove(const FInputActionValue& InputActionValue);
	void CameraRotate(const FInputActionValue& InputActionValue);
	void CameraReset();
	void CameraZoom(const FInputActionValue& InputActionValue);

	FVector CameraLocation;

	FRotator CameraRotation;

	FVector RotateLocation;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraBaseHeight = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraBaseAngle = -70.f;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraMS = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraRS = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraRadius = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraZoomSpeed = 1.f;
	
	void SetCameraLocation();
	void SetRotateLocation();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
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
	TObjectPtr<UInputAction> CameraResetAction;

	UPROPERTY()
	TObjectPtr<UZAbilitySystemComponent> ZAbilitySystemComponent;

	UZAbilitySystemComponent* GetASC();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void CameraMove(const FInputActionValue& InputActionValue);
	void CameraReset();

	FVector CameraLocation;

	UPROPERTY(EditDefaultsOnly)
	FVector CameraBaseLocation = FVector(-30.f, 0.f, 1000.f);

	UPROPERTY(EditDefaultsOnly)
	float CameraMS = 3.f;
};

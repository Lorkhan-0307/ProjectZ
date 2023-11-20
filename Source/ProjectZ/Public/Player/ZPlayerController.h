// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ZPlayerController.generated.h"

class UCardHandHUD;
class UInputMappingContext;
class UInputAction;
class UCardComponent;

UCLASS()
class PROJECTZ_API AZPlayerController : public APlayerController
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
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> TestAction;

	UPROPERTY(EditAnywhere, Category = Card)
	TSubclassOf<UCardHandHUD> CardHandHUDClass;
	
	void Move(const FInputActionValue& InputActionValue);

	void Look(const FInputActionValue& InputActionValue);

	void Test(); 

	UPROPERTY()
	UCardComponent* CardComponent;
};



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

UCLASS()
class PROJECTZ_API AZPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY()
	UCardComponent* CardComponent;

	UPROPERTY()
	UZInputComponent* ZInputComponent;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UZInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UZAbilitySystemComponent> ZAbilitySystemComponent;

	UZAbilitySystemComponent* GetASC();
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
};

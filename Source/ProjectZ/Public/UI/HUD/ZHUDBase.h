// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/ZWidgetController.h"
#include "ZHUDBase.generated.h"

class UInventoryWidgetController;
class UOverlayWidgetController;
class AZNonCombatCharacter;
class UZUserWidget;
class UZWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class APlayerState;
class APlayerController;
class AZPlayerCharacter;

UCLASS()
class PROJECTZ_API AZHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, AZPlayerCharacter* Character);

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UInventoryWidgetController* GetInventoryWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	UPROPERTY()
	TObjectPtr<UZUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UZUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UZWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UZUserWidget> InventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UZUserWidget> InventoryWidgetClass;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UZWidgetController> InventoryWidgetControllerClass;

protected:

private:
	UPROPERTY()
	AZPlayerCharacter* ZCharacter;
};

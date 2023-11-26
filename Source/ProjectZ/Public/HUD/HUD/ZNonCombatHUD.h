// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/HUD/ZHUDBase.h"
#include "ZNonCombatHUD.generated.h"

class UNonCombatOverlayWidgetController;
class UNonCombatOverlay;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UAuraUserWidget;

UCLASS()
class PROJECTZ_API AZNonCombatHUD : public AZHUDBase
{
	GENERATED_BODY()

public:
	//UPROPERTY()
	//TObjectPtr<UNonCombatOverlay> NonCombatOverlayWidget;

	UNonCombatOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	virtual void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) override;

protected:
	virtual void BeginPlay() override;

private:
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<UNonCombatOverlay> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UNonCombatOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UNonCombatOverlayWidgetController> OverlayWidgetControllerClass;
};


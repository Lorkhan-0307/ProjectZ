// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZHUDBase.generated.h"

class AZCharacter;
class UZUserWidget;
class UZWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class APlayerState;
class APlayerController;

UCLASS()
class PROJECTZ_API AZHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UZUserWidget> OverlayWidget;

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, AZCharacter* Character);

	virtual UZWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UZUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UZWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UZWidgetController> OverlayWidgetControllerClass;

private:
	UPROPERTY()
	AZCharacter* ZCharacter;
};

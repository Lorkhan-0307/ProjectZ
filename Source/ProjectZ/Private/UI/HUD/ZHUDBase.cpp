// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/HUD/ZHUDBase.h"

#include "Character/ZCharacter.h"
#include "Player/ZPlayerController.h"
#include "UI/ZUserWidget.h"
#include "Ui/WidgetController/ZWidgetController.h"

UZWidgetController* AZHUDBase::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UZWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		//OverlayWidgetController->BindCallbacksToDependencies();

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AZHUDBase::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, AZCharacter* Character)
{
	ZCharacter = Character;
	OverlayWidget = CreateWidget<UZUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetCardComponent(Character->CardComponent);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UZWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}

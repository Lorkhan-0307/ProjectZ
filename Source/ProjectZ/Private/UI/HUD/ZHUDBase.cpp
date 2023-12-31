// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/HUD/ZHUDBase.h"

#include "Character/ZNonCombatCharacter.h"
#include "Player/ZNonCombatPlayerController.h"
#include "UI/ZUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Ui/WidgetController/ZWidgetController.h"

UOverlayWidgetController* AZHUDBase::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		OverlayWidgetController->BindCallbacksToDependencies();

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

// Create Overlay and Assign OverlayWidgetController to Overlay
void AZHUDBase::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, AZPlayerCharacter* Character)
{
	if (OverlayWidget != nullptr) return;
	ZCharacter = Character;
	OverlayWidget = CreateWidget<UZUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetCardComponent(Character->CardComponent); // Code of CardHandHUD moved here

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UZWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues(); // Set Initial values to Overlay

	OverlayWidget->AddToViewport();
}
